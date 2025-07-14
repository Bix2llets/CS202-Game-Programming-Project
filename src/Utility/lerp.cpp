#include "Utility/lerp.hpp"

ColorMixer::Oklab ColorMixer::rgbToOklab(sf::Color color) {
    // sRGB to linear
    auto toLinear = [](float c) {
        return c <= 0.04045f ? c / 12.92f
                             : std::pow((c + 0.055f) / 1.055f, 2.4f);
    };
    int r = toLinear(static_cast<float>(color.r) / 255);
    int g = toLinear(static_cast<float>(color.g) / 255);
    int b = toLinear(static_cast<float>(color.b) / 255);

    // Linear RGB to LMS
    float l = 0.4122214708f * r + 0.5363325363f * g + 0.0514459929f * b;
    float m = 0.2119034982f * r + 0.6806995451f * g + 0.1073969566f * b;
    float s = 0.0883024619f * r + 0.2817188376f * g + 0.6299787005f * b;

    // Cube root
    l = std::cbrt(l);
    m = std::cbrt(m);
    s = std::cbrt(s);

    // LMS to Oklab
    return {0.2104542553f * l + 0.7936177850f * m - 0.0040720468f * s,
            1.9779984951f * l - 2.4285922050f * m + 0.4505937099f * s,
            0.0259040371f * l + 0.7827717662f * m - 0.8086757660f * s};
}

sf::Color ColorMixer::oklabToRgb(const ColorMixer::Oklab &lab) {
    float l = lab.L + 0.3963377774f * lab.a + 0.2158037573f * lab.b;
    float m = lab.L - 0.1055613458f * lab.a - 0.0638541728f * lab.b;
    float s = lab.L - 0.0894841775f * lab.a - 1.2914855480f * lab.b;

    l = l * l * l;
    m = m * m * m;
    s = s * s * s;

    // LMS to linear RGB
    float r = +4.0767416621f * l - 3.3077115913f * m + 0.2309699292f * s;
    float g = -1.2684380046f * l + 2.6097574011f * m - 0.3413193965f * s;
    float b = -0.0041960863f * l - 0.7034186147f * m + 1.7076147010f * s;

    // Linear to sRGB
    auto toSrgb = [](float c) {
        return c <= 0.0031308f ? 12.92f * c
                               : 1.055f * std::pow(c, 1.0f / 2.4f) - 0.055f;
    };
    r = std::clamp(toSrgb(r), 0.0f, 1.0f);
    g = std::clamp(toSrgb(g), 0.0f, 1.0f);
    b = std::clamp(toSrgb(b), 0.0f, 1.0f);

    return sf::Color(static_cast<std::uint32_t>(r * 255),
                     static_cast<std::uint32_t>(g * 255),
                     static_cast<std::uint32_t>(b * 255), 255);
}

sf::Color ColorMixer::perceptualLerp(const sf::Color &start,
                                     const sf::Color &end, float t) {
    t = std::clamp(t, 0.0f, 1.0f);
    Oklab labA = rgbToOklab(start);
    Oklab labB = rgbToOklab(end);

    Oklab labResult;
    labResult.L = labA.L + (labB.L - labA.L) * t;
    labResult.a = labA.a + (labB.a - labA.a) * t;
    labResult.b = labA.b + (labB.b - labA.b) * t;

    sf::Color result = oklabToRgb(labResult);
    result.a = start.a + (end.a - start.a) * t;
    return result;
}
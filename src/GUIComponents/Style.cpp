#include "GUIComponents/Style.hpp"

#include "Core/ResourceManager.hpp"
void Style::loadJson(const nlohmann::json &file,
                     const ResourceManager &manager) {
    nlohmann::json style = file["style"];
    nlohmann::json normalColor = style["normal"];
    nlohmann::json hoverColor = style["hover"];
    nlohmann::json clickColor = style["click"];
    auto hexConverter = [](std::string hexCode) {
        if (hexCode.size() == 7 && hexCode[0] == '#') {
            unsigned int colorValue =
                std::stoul(hexCode.substr(1), nullptr, 16);
            return sf::Color(colorValue);
        }
    };

    normal.background =
        hexConverter(normalColor["background"].get<std::string>());
    normal.border = hexConverter(normalColor["border"].get<std::string>());
    normal.text = hexConverter(normalColor["text"].get<std::string>());

    hover.background =
        hexConverter(hoverColor["background"].get<std::string>());
    hover.border = hexConverter(hoverColor["border"].get<std::string>());
    hover.text = hexConverter(hoverColor["text"].get<std::string>());

    click.background =
        hexConverter(clickColor["background"].get<std::string>());
    click.border = hexConverter(clickColor["border"].get<std::string>());
    click.text = hexConverter(clickColor["text"].get<std::string>());

    borderRadius = style["borderRadius"].get<int>();
    borderWidth = style["borderWidth"].get<int>();

    font = manager.getFont(style["font"].get<std::string>());
    padding = style["padding"].get<std::array<int, 2>>();

    id = style["id"].get<std::string>();
}
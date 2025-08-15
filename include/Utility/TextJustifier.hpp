#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <algorithm>

class TextJustifier {
public:
    static sf::Text createJustified(
        const std::string& text,
        const sf::Font& font,
        unsigned int charSize,
        float targetWidth,
        sf::Color color = sf::Color::White
    ) {
        sf::Text measurer(font);
        measurer.setCharacterSize(charSize);
        measurer.setString("");
        measurer.setFillColor(color);

        auto wordWidth = [&](const std::string& s) {
            measurer.setString(s);
            return measurer.getLocalBounds().size.x;
        };
        float spaceWidth = wordWidth(" ");

        // Split words
        std::vector<std::string> words;
        {
            std::istringstream iss(text);
            std::string w;
            while (iss >> w) words.push_back(w);
        }

        // DP
        int n = (int)words.size();
        std::vector<double> dp(n + 1, 1e18);
        std::vector<int> nextBreak(n + 1, -1);
        dp[n] = 0;
        auto badness = [](float slack) { return (slack < 0) ? 1e9 : slack*slack*slack; };

        for (int i = n - 1; i >= 0; --i) {
            float lineWidth = 0;
            for (int j = i; j < n; ++j) {
                if (j == i) lineWidth = wordWidth(words[j]);
                else lineWidth += spaceWidth + wordWidth(words[j]);
                if (lineWidth > targetWidth) break;
                bool lastLine = (j == n - 1);
                double cost = lastLine ? 0 : badness(targetWidth - lineWidth);
                double cand = cost + dp[j + 1];
                if (cand < dp[i]) {
                    dp[i] = cand;
                    nextBreak[i] = j + 1;
                }
            }
        }

        // Build final justified string
        std::ostringstream oss;
        for (int i = 0; i < n; ) {
            int j = nextBreak[i];
            if (j <= i) break;

            // measure line width
            float naturalWidth = 0.f;
            for (int k = i; k < j; ++k) naturalWidth += wordWidth(words[k]);
            int gaps = j - i - 1;
            naturalWidth += gaps * spaceWidth;

            float slack = targetWidth - naturalWidth;
            float extraSpacePerGap = (gaps > 0 && j != n) ? slack / gaps : 0.f;
            int extraSpacesPerGap = (extraSpacePerGap > 0) ? (int)std::round(extraSpacePerGap / spaceWidth) : 0;

            for (int k = i; k < j; ++k) {
                oss << words[k];
                if (k != j - 1) {
                    oss << " ";
                    oss << std::string(extraSpacesPerGap, ' ');
                }
            }
            oss << "\n";
            i = j;
        }

        sf::Text out(font);
        out.setString(oss.str());
        out.setCharacterSize(charSize);
        out.setFillColor(color);
        out.setLineSpacing(1.5f);
        return out;
    }
};
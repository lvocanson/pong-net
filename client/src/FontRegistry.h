#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

#define FONT_DEFAULT "JuliaMono-Regular.ttf"

class FontRegistry
{
public:
    FontRegistry();
    ~FontRegistry() = delete;

    sf::Font* GetFont(const std::string& fontName = FONT_DEFAULT);
    void LoadFont(const std::string& fontName);

    void ClearFonts();

private:
    std::unordered_map<std::string, sf::Font*> m_Fonts;
};

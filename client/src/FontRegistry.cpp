#include "FontRegistry.h"
#include <SFML/Graphics/Font.hpp>

FontRegistry::FontRegistry()
{
    m_Fonts = std::unordered_map<std::string, sf::Font*>();
}

sf::Font* FontRegistry::GetFont(const std::string& fontName)
{
    if (!m_Fonts.contains(fontName))
    {
        throw std::runtime_error("Font not registered: " + fontName);
    }

    return m_Fonts.at(fontName);
}

void FontRegistry::LoadFont(const std::string& fontName)
{
    auto* font = new sf::Font;

    try
    {
        if (!font->openFromFile("res/fonts/" + fontName))
        {
            throw std::runtime_error("Failed to load font: " + fontName);
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception caught: " << e.what() << std::endl;

        delete font;

        throw;
    }

    m_Fonts.insert(std::pair(fontName, font));
}

void FontRegistry::ClearFonts()
{
    for (auto& [string, font] : m_Fonts)
    {
        delete font;
    }

    m_Fonts.clear(); // Efface le contenu du map
}

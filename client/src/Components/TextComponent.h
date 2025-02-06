#pragma once
#include "BaseComponent.h"

#define TEXT_SIZE 50
#define TEXT_COLOR sf::Color::White
#define TEXT_DEFAULT_ALIGNEMENT TextAlignment::Center

enum TextAlignment
{
    Left,
    Center,
    Right
};

class TextComponent : public BaseComponent
{
public:

    TextComponent(const sf::Font& font);
    TextComponent(const std::string& text, sf::Font& font, BaseComponent& parentComponent, const sf::Color& color, unsigned int size, TextAlignment alignment);

    void Update(float dt) override;

    void SetPosition(const sf::Vector2f& position) override;
    void SetTextAlignment(TextAlignment alignment);
    void SetPositionCentered(const sf::Vector2f& position);
    void SetText(const std::string& text);
    void SetColor(const sf::Color& color);
    void SetCharacterSize(unsigned int size);

    sf::Vector2f GetPosition() const override;
    const sf::Text& GetText() const;
    sf::Vector2f GetSize() const override { return m_Text.getLocalBounds().size; }
    unsigned int GetCharacterSize() const { return m_Text.getCharacterSize(); }


private:
    sf::Text m_Text;
    TextAlignment m_Alignment;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};


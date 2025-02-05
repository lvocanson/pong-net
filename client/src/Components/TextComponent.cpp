#include "TextComponent.h"
//#include "src/core/Managers/Resources/FontRegistry.h"

TextComponent::TextComponent(sf::Font& font)
    : m_Alignment(Center), 
    m_Text(font, "", 50u)
{
    m_Text.setString("");
    //m_Text.setFont(*FontRegistry::GetFont("coolvetica.otf"));
    m_Text.setFillColor(sf::Color::White);
    m_Text.setCharacterSize(20);

    SetPosition(sf::Vector2f(0, 0));
}

TextComponent::TextComponent(const std::string& text, sf::Font& font, BaseComponent* parentComponent, const sf::Color& color, unsigned int size, TextAlignment alignment)
    : m_Alignment(alignment),
    m_Text(font, "", 50u)
{
    m_ParentComponent = parentComponent;
    m_Text.setString(text);
    //m_Text.setFont(*FontRegistry::GetFont("coolvetica.otf"));
    m_Text.setFillColor(color);
    m_Text.setCharacterSize(size);

    SetPosition(m_ParentComponent->GetPosition());
}

void TextComponent::Update(float dt)
{

}

void TextComponent::SetTextAlignment(TextAlignment alignment)
{
    m_Alignment = alignment;
}

void TextComponent::SetPosition(const sf::Vector2f& position)
{
    if (m_Alignment == TextAlignment::Center)
    {
        SetPositionCentered(position);
    }
    else if (m_Alignment == TextAlignment::Left)
    {
        m_Text.setPosition(sf::Vector2f(position.x, position.y));
    }
    else
    {
        m_Text.setPosition(position);
    }
}

void TextComponent::SetText(const std::string& text)
{
    m_Text.setString(text);
}

void TextComponent::SetColor(const sf::Color& color)
{
    m_Text.setFillColor(color);
}

void TextComponent::SetCharacterSize(unsigned int size)
{
    m_Text.setCharacterSize(size);
}

void TextComponent::SetPositionCentered(const sf::Vector2f& position)
{
    sf::FloatRect bounds = m_Text.getLocalBounds();
    sf::Vector2f newOrigin = sf::Vector2f(bounds.position.x + bounds.size.x / 2.0f, bounds.position.y + bounds.size.y / 2.0f);
    m_Text.setOrigin(newOrigin);
    sf::Vector2f newPosition = sf::Vector2f(position.x + bounds.size.x / 2.0f, position.y + bounds.size.y / 2.0f);
    m_Text.setPosition(newPosition);
}

void TextComponent::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_Text, states);
}

sf::Vector2f TextComponent::GetPosition() const
{
    return m_Text.getPosition();
}

const sf::Text& TextComponent::GetText() const
{
    return m_Text;
}

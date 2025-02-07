#include "Text.h"
#include "Utils/EnumOperators.h"

Text::Text(const sf::Font& font)
	: m_Text(font)
	, m_Alignment(TextAlignment::Center)
{
}

void Text::SetString(const sf::String& string)
{
    auto pos = m_Text.getPosition();
    m_Text.setString(string);
    m_Text.setPosition(pos);
}

void Text::SetAlignment(TextAlignment alignment)
{
    m_Alignment = alignment;
    const auto oldOrigin = m_Text.getOrigin();
    const auto bounds = m_Text.getGlobalBounds();

    sf::Vector2f newOrigin = oldOrigin;
    using enum TextAlignment;

    if ((alignment & VTop) != None)
    {
        newOrigin.y = bounds.position.y;
    }
    else if ((alignment & VCenter) != None)
    {
        newOrigin.y = bounds.position.y + bounds.size.y / 2.f;
    }
    else if ((alignment & VBottom) != None)
    {
        newOrigin.y = bounds.position.y + bounds.size.y;
    }

    if ((alignment & HLeft) != None)
    {
        newOrigin.x = bounds.position.x;
    }
    else if ((alignment & HCenter) != None)
    {
        newOrigin.x = bounds.position.x + bounds.size.x / 2.f;
    }
    else if ((alignment & HRight) != None)
    {
        newOrigin.x = bounds.position.x + bounds.size.x;
    }

    m_Text.setOrigin(newOrigin);
    m_Text.setPosition(bounds.getCenter());
}

void Text::Draw(sf::RenderWindow& wnd) const
{
	wnd.draw(m_Text);
}

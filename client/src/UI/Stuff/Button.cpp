#include "Button.h"

inline constexpr sf::Color InterractionTint = {40, 40, 40, 0};

Button::Button()
	: m_Background()
	, m_Color(sf::Color::White)
	, m_Status{}
	, m_Callback()
{
}

void Button::SetRect(const sf::FloatRect& rect)
{
	m_Background.setSize(rect.size);
	m_Background.setOrigin(m_Background.getLocalBounds().getCenter());
	m_Background.setPosition(rect.position);
}

void Button::SetBgColor(sf::Color color)
{
	m_Color = color;
	m_Background.setFillColor(color);
}

void Button::Draw(sf::RenderWindow& wnd) const
{
	wnd.draw(m_Background);
}

void Button::OnEvent(const sf::Event& event)
{
	if (auto* e = event.getIf<sf::Event::MouseMoved>())
	{
		if (m_Background.getGlobalBounds().contains(sf::Vector2f{e->position}))
		{
			if (m_Status == Released)
			{
				m_Status = Hovered;
				m_Background.setFillColor(m_Color + InterractionTint);
			}
		}
		else
		{
			m_Status = Released;
			m_Background.setFillColor(m_Color);
		}
	}

	else if (auto* e = event.getIf<sf::Event::MouseButtonPressed>())
	{
		if (m_Background.getGlobalBounds().contains(sf::Vector2f{e->position}))
		{
			m_Status = Held;
			m_Background.setFillColor(m_Color - InterractionTint);
		}
	}

	else if (auto* e = event.getIf<sf::Event::MouseButtonReleased>())
	{
		if (m_Status == Held && m_Background.getGlobalBounds().contains(sf::Vector2f{e->position}))
		{
			m_Status = Hovered;
			m_Background.setFillColor(m_Color);
			m_Callback();
		}
	}
}

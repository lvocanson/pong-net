#include "Button.h"

inline constexpr sf::Color InterractionTint = {40, 40, 40, 0};

Button::Button()
	: m_Backgroud()
	, m_Color(sf::Color::White)
	, m_Status{}
	, m_Callback()
{
}

void Button::SetRect(const sf::FloatRect& rect)
{
	m_Backgroud.setSize(rect.size);
	m_Backgroud.setOrigin(m_Backgroud.getLocalBounds().getCenter());
	m_Backgroud.setPosition(rect.position);
}

void Button::SetBgColor(sf::Color color)
{
	m_Color = color;
	m_Backgroud.setFillColor(color);
}

void Button::Draw(sf::RenderWindow& wnd) const
{
	wnd.draw(m_Backgroud);
}

void Button::OnEvent(const sf::Event& event)
{
	if (auto* e = event.getIf<sf::Event::MouseMoved>())
	{
		if (m_Backgroud.getGlobalBounds().contains(sf::Vector2f{e->position}))
		{
			m_Status = Hovered;
			m_Backgroud.setFillColor(m_Color + InterractionTint);
		}
		else if (m_Status == Hovered)
		{
			m_Status = Released;
			m_Backgroud.setFillColor(m_Color);
		}
	}

	else if (auto* e = event.getIf<sf::Event::MouseButtonPressed>())
	{
		if (m_Backgroud.getGlobalBounds().contains(sf::Vector2f{e->position}))
		{
			m_Status = Held;
			m_Backgroud.setFillColor(m_Color - InterractionTint);
		}
	}

	else if (auto* e = event.getIf<sf::Event::MouseButtonReleased>())
	{
		if (m_Status == Held && m_Backgroud.getGlobalBounds().contains(sf::Vector2f{e->position}))
		{
			m_Status = Hovered;
			m_Backgroud.setFillColor(m_Color);
			m_Callback();
		}
	}
}

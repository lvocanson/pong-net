#include "TextField.h"
#include "Utils/Misc.h"

inline constexpr float TimeBetweenBlinks = 0.6f;

TextField::TextField(const sf::Font& font)
	: m_Text(font)
	, m_Editing(false)
	, m_BlinkingTimer(0)
	, m_BlinkOn(false)
{
	m_Background.setFillColor(sf::Color::White);
	m_Background.setOutlineColor(sf::Color::Black);
	m_Background.setOutlineThickness(-2.f);

	auto textSize = static_cast<float>(m_Text.getCharacterSize());

	m_Blinker.setSize({5.f, textSize});
	m_Blinker.setOrigin({-5.f, 0.f});

	m_Text.setOrigin({0, textSize / 2.f});
	m_Text.setFillColor(sf::Color::Black);

	UpdateBlinkerColor();
}

void TextField::SetRect(const sf::FloatRect& rect)
{
	m_Background.setSize(rect.size);
	m_Background.setOrigin(m_Background.getLocalBounds().getCenter());
	m_Background.setPosition(rect.position);

	auto newBounds = m_Background.getGlobalBounds();
	m_Text.setPosition({newBounds.position.x, newBounds.position.y + newBounds.size.y / 2.f});
	UpdateBlinkerPos();
}

void TextField::SetString(const sf::String& string)
{
	m_Text.setString(string);
	m_String = string;
	UpdateBlinkerPos();
}

void TextField::UpdateBlinkerColor()
{
	if (m_BlinkOn)
	{
		m_Blinker.setFillColor(sf::Color::Black);
	}
	else
	{
		m_Blinker.setFillColor(sf::Color::Transparent);
	}
}

void TextField::UpdateBlinkerPos()
{
	auto bounds = m_Text.getGlobalBounds();
	m_Blinker.setPosition(bounds.position + sf::Vector2{bounds.size.x, 0.f});
}

void TextField::Draw(sf::RenderWindow& wnd) const
{
	wnd.draw(m_Background);
	wnd.draw(m_Blinker);
	wnd.draw(m_Text);
}

void TextField::OnUpdate(float dt)
{
	if (!m_Editing)
		return;

	m_BlinkingTimer += dt;
	if (m_BlinkingTimer > TimeBetweenBlinks)
	{
		m_BlinkingTimer -= TimeBetweenBlinks;
		m_BlinkOn = !m_BlinkOn;

		UpdateBlinkerColor();
	}
}

void TextField::OnEvent(const sf::Event& event)
{
	if (auto* e = event.getIf<sf::Event::MouseButtonPressed>())
	{
		m_Editing = m_BlinkOn = m_Background.getGlobalBounds().contains(sf::Vector2f{e->position});
		m_BlinkingTimer = 0.f;
		UpdateBlinkerColor();
	}

	if (m_Editing)
	{
		if (auto* e = event.getIf<sf::Event::KeyPressed>())
		{
			using enum sf::Keyboard::Key;
			if (e->code == Enter || e->code == Escape)
			{
				m_Editing = false;
			}
			else if (e->code == Backspace)
			{
				size_t size = m_String.getSize();
				m_String.erase(size - 1);
				m_Text.setString(m_String);
				UpdateBlinkerPos();
			}
		}
		else if (auto* e = event.getIf<sf::Event::TextEntered>())
		{
			if (!Misc::IsValidUnicodeChar(e->unicode))
				// Can't print this char
				return;

			m_String += e->unicode;
			m_Text.setString(m_String);
			UpdateBlinkerPos();
		}
	}
}

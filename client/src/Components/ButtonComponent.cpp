#include "ButtonComponent.h"
#include <Window/InputHandler.h>

ButtonComponent::ButtonComponent(const sf::Font& font, const InputHandler& inputHandler)
	: m_IdleColor(sf::Color::White)
	, m_OnClickCallback(nullptr)
	, m_Input(inputHandler)
	, m_Text(font)
{
}

void ButtonComponent::Update(float dt)
{
	if (IsMouseOver())
	{
		m_Shape.setFillColor(m_IdleColor - sf::Color{50, 50, 50, 0});

		if (m_Input.IsMouseButtonPressed(sf::Mouse::Button::Left) && m_OnClickCallback)
		{
			m_OnClickCallback();
			m_Shape.setFillColor(m_IdleColor + sf::Color{50, 50, 50, 0});
		}
	}
	else
	{
		m_Shape.setFillColor(m_IdleColor);
	}
}

void ButtonComponent::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_Shape, states);
	target.draw(m_Text, states);
}

bool ButtonComponent::IsMouseOver()
{
	sf::Vector2f mousePos = (sf::Vector2f)m_Input.GetMousePosition();
	sf::Vector2f buttonPos = m_Shape.getPosition();
	sf::Vector2f buttonSize = m_Shape.getSize();

	float Right = buttonPos.x + buttonSize.x;
	float Bottom = buttonPos.y + buttonSize.y;

	return mousePos.x >= buttonPos.x && mousePos.x <= Right && mousePos.y >= buttonPos.y && mousePos.y <= Bottom;
}

void ButtonComponent::SetPosition(const sf::Vector2f& position)
{
	m_Shape.setPosition(position);

	sf::FloatRect buttonBounds = m_Shape.getLocalBounds();
	sf::Vector2f buttonTextPosition = position;
	buttonTextPosition.x += (buttonBounds.size.x - m_Text.GetSize().x) / 2.0f;
	buttonTextPosition.y += (buttonBounds.size.y - m_Text.GetSize().y) / 2.0f;

	m_Text.SetPosition(buttonTextPosition);
}

sf::Vector2f ButtonComponent::GetPosition() const
{
	return m_Shape.getPosition();
}

void ButtonComponent::SetSize(sf::Vector2f size)
{
	m_Shape.setSize(size);
}

sf::Vector2f ButtonComponent::GetSize() const
{
	return m_Shape.getSize();
}

void ButtonComponent::SetColor(sf::Color color)
{
	m_IdleColor = color;
	m_Shape.setFillColor(color);
}

const TextComponent& ButtonComponent::GetTextComponent() const
{
	return m_Text;
}

void ButtonComponent::SetOnClickCallback(std::function<void()>& callback)
{
	m_OnClickCallback = callback;
}

void ButtonComponent::SetButtonText(const std::string& text, const sf::Font& font, const sf::Color& textColor, unsigned int textSize, TextAlignment textAlignment)
{
	m_Text.SetText(text);
	m_Text.SetColor(textColor);
	m_Text.SetCharacterSize(textSize);
	m_Text.SetTextAlignment(textAlignment);

	sf::Vector2f buttonPosition = GetPosition();
	sf::Vector2f buttonSize = GetSize();
	sf::Vector2f textPosition = buttonPosition + sf::Vector2f((buttonSize.x - m_Text.GetSize().x) / 2.0f, (buttonSize.y - m_Text.GetSize().y) / 2.0f);
	m_Text.SetPosition(textPosition);
}

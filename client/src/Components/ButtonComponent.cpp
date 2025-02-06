#include "ButtonComponent.h"
#include <Window/InputHandler.h>

ButtonComponent::ButtonComponent(const sf::Font& font, const InputHandler& inputHandler)
	: m_IdleColor(sf::Color::White)
	, m_OnClickCallback(nullptr)
	, m_Input(inputHandler)
	, m_Text(font)
{
}

void ButtonComponent::Update(float dt, Window& window)
{
	if (IsMouseOver(window))
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

	m_Text.Update(dt, window);
}

void ButtonComponent::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_Shape, states);
	target.draw(m_Text, states);
}

bool ButtonComponent::IsMouseOver(Window& window)
{
	return m_Shape.getGlobalBounds().contains(sf::Vector2f(m_Input.GetMousePosition(window)));
}

void ButtonComponent::SetPosition(const sf::Vector2f& position)
{
	m_Shape.setPosition(position);
	m_Text.SetPosition(position);
}

sf::Vector2f ButtonComponent::GetPosition() const
{
	return m_Shape.getPosition();
}

void ButtonComponent::SetSize(sf::Vector2f size)
{
	m_Shape.setSize(size);
	m_Shape.setOrigin(size / 2.f);
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

void ButtonComponent::SetButtonText(const std::string& text, const sf::Font& font, const sf::Color& textColor, unsigned int textSize)
{
	m_Text.SetCharacterSize(textSize);
	m_Text.SetColor(textColor);
	m_Text.SetText(text);
}

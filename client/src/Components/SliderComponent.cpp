#include "SliderComponent.h"
#include <Window/InputHandler.h>
#include <fstream>
#include <iostream>
#include <filesystem>

SliderComponent::SliderComponent(const InputHandler& inputHandler, sf::Music& music)
	: m_Input(inputHandler), m_Music(music), m_Filename("Save.txt")
{
	m_Bar.setFillColor(sf::Color::White);
	m_Knob.setRadius(10);
	m_Knob.setFillColor(sf::Color::Red);
	m_Knob.setOrigin(sf::Vector2(m_Knob.getRadius(), m_Knob.getRadius()));

	m_Value = LoadVolumeValue();
	if (m_Value == -1)
	{
		m_Value = m_Music.getVolume();
	}
	else
	{
		m_Music.setVolume(m_Value);
	}
}

void SliderComponent::Update(float dt)
{
	sf::Vector2i mousePos = m_Input.GetMousePosition();

	float sliderLeft = m_Bar.getPosition().x;
	float sliderRight = m_Bar.getPosition().x + m_Bar.getSize().x;

	if (IsMouseOver())
	{
		if (m_Input.IsMouseButtonPressed(sf::Mouse::Button::Left))
		{
			m_IsDragging = true;
		}
	}
	if (m_Input.IsMouseButtonReleased(sf::Mouse::Button::Left))
	{
		m_IsDragging = false;
	}
	if (m_IsDragging)
	{
		float newX = std::max(sliderLeft, std::min(static_cast<float>(mousePos.x), sliderRight));
		m_Knob.setPosition(sf::Vector2f(newX, m_Knob.getPosition().y));

		// Mise � jour de la valeur du slider
		m_Value = ((newX - sliderLeft) / m_Bar.getSize().x) * 100;
		m_Music.setVolume(m_Value);
	}
}

void SliderComponent::SaveVolumeValue() const
{
	std::ofstream file(m_Filename);
	if (file)
	{
		file << m_Music.getVolume();
		file.close();
	}
}

float SliderComponent::LoadVolumeValue() const
{
	std::ifstream file(m_Filename);
	float m_Value = -1;
	if (file)
	{
		file >> m_Value;
		file.close();
	}
	return m_Value;
}

void SliderComponent::SetPosition(const sf::Vector2f& pos)
{
	m_Bar.setPosition(pos);
	m_Knob.setPosition(sf::Vector2(pos.x, pos.y + m_Bar.getSize().y / 2));
}

sf::Vector2f SliderComponent::GetPosition() const
{
	return m_Bar.getPosition();
}

void SliderComponent::SetWidth(float width)
{
	m_Bar.setSize(sf::Vector2f(width, 5));
	m_Knob.setPosition(sf::Vector2f(m_Bar.getPosition().x + (m_Bar.getSize().x * (m_Value / 100)), m_Knob.getPosition().y));
}

sf::Vector2f SliderComponent::GetSize() const
{
	return m_Bar.getSize();
}

bool SliderComponent::IsMouseOver()
{
	sf::Vector2f mousePos = (sf::Vector2f)m_Input.GetMousePosition();
	sf::Vector2f knobPos = m_Knob.getPosition();
	float knobSize = m_Knob.getRadius();

	float Right = knobPos.x + knobSize;
	float Left = knobPos.x - knobSize;
	float Bottom = knobPos.y + knobSize;
	float Top = knobPos.y - knobSize;

	return mousePos.x >= Left && mousePos.x <= Right && mousePos.y >= Top && mousePos.y <= Bottom;
}

void SliderComponent::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_Bar);
	target.draw(m_Knob);
}
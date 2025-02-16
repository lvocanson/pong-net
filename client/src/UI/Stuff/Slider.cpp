#include "Slider.h"

Slider::Slider()
	: m_Value(0.5f)
	, m_MinValue(0.f)
	, m_MaxValue(1.f)
	, m_Status(Released)
{
	m_Bar.setOutlineColor(sf::Color::White - sf::Color(20, 0, 0, 20));
	m_Bar.setOutlineThickness(1.f);

	m_Knob.setFillColor(sf::Color::Red);
	m_Knob.setOutlineColor(sf::Color::Red - sf::Color(20, 0, 0, 20));
	m_Knob.setOutlineThickness(1.f);
}

void Slider::SetRect(const sf::FloatRect& rect)
{
	m_Bar.setSize(rect.size);
	m_Bar.setPosition(rect.position);

	auto bounds = m_Bar.getGlobalBounds();
	auto center = bounds.getCenter();
	float remapedValue = (m_Value - m_MinValue) / m_MaxValue;
	m_Knob.setPosition({remapedValue * bounds.size.x + bounds.position.x, center.y});
}

void Slider::SetKnobRadius(float radius)
{
	m_Knob.setOrigin({radius, radius});
	m_Knob.setRadius(radius);
}

void Slider::SetValue(float value)
{
	m_Value = std::clamp(value, m_MinValue, m_MaxValue);
	float remapedValue = (m_Value - m_MinValue) / m_MaxValue;

	auto bounds = m_Bar.getGlobalBounds();
	auto center = bounds.getCenter();
	m_Knob.setPosition({remapedValue * bounds.size.x + bounds.position.x, center.y});
}

void Slider::SetLimits(float min, float max)
{
	m_Value = std::clamp(m_Value, min, max);
	m_MinValue = min;
	m_MaxValue = max;
}

void Slider::Draw(sf::RenderWindow& wnd) const
{
	wnd.draw(m_Bar);
	wnd.draw(m_Knob);
}

void Slider::OnEvent(const sf::Event& event)
{
	if (auto* e = event.getIf<sf::Event::MouseMoved>())
	{
		if (m_Status == Held)
		{
			auto bounds = m_Bar.getGlobalBounds();
			float pixelValue = float(e->position.x) - bounds.position.x;
			SetValue(m_MinValue + pixelValue * m_MaxValue / bounds.size.x);
			m_Callback(m_Value);
		}
		else
		{
			auto distance = m_Knob.getPosition() - sf::Vector2f(e->position);
			float radius = m_Knob.getRadius();
			if (distance.lengthSquared() < radius * radius)
			{
				m_Status = Hovered;
			}
			else
			{
				m_Status = Released;
			}
		}
	}

	else if (auto* e = event.getIf<sf::Event::MouseButtonPressed>())
	{
		if (m_Status == Hovered)
		{
			m_Status = Held;
		}
	}

	else if (auto* e = event.getIf<sf::Event::MouseButtonReleased>())
	{
		m_Status = Released;
	}
}

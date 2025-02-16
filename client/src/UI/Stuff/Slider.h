#pragma once

#include "UI/IDrawable.h"
#include "UI/IEventConsumer.h"
#include <SFML/Graphics.hpp>
#include <functional>

class Slider : public IDrawable, public IEventConsumer
{
public:

	Slider();

	void SetRect(const sf::FloatRect& rect);
	void SetKnobRadius(float radius);
	void SetValue(float value);
	void SetLimits(float min, float max);
	void SetCallback(std::function<void(float)> callback) { m_Callback = callback; }

protected:

	// Inherited via IDrawable
	void Draw(sf::RenderWindow& wnd) const override;

	// Inherited via IEventConsumer
	void OnEvent(const sf::Event& event) override;

protected:

	sf::RectangleShape m_Bar;
	sf::CircleShape m_Knob;
	float m_Value;
	float m_MinValue;
	float m_MaxValue;
	enum : char
	{
		Released = 0,
		Hovered,
		Held,
	} m_Status;
	std::function<void(float)> m_Callback;
};

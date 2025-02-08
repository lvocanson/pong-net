#pragma once
#include "UI/IDrawable.h"
#include "UI/IEventConsumer.h"
#include <SFML/Graphics.hpp>
#include <functional>

class Button : public IDrawable, public IEventConsumer
{
public:
	
	Button();

	void SetRect(const sf::FloatRect& rect);
	void SetBgColor(sf::Color color);
	void SetCallback(std::function<void()> callback) { m_Callback = callback; }

protected:

	// Inherited via IDrawable
	void Draw(sf::RenderWindow& wnd) const override;

	// Inherited via IEventConsumer
	void OnEvent(const sf::Event& event) override;

protected:

	sf::RectangleShape m_Background;
	sf::Color m_Color;
	enum : char
	{
		Released = 0,
		Hovered,
		Held,
	} m_Status;
	std::function<void()> m_Callback;
};

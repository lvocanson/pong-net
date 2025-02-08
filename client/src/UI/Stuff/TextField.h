#pragma once
#include "UI/IDrawable.h"
#include "UI/IUpdatable.h"
#include "UI/IEventConsumer.h"
#include <SFML/Graphics.hpp>

class TextField : public IDrawable, public IUpdatable, public IEventConsumer
{
public:

	TextField(const sf::Font& font);

	void SetRect(const sf::FloatRect& rect);
	void SetString(const sf::String& string);
	sf::String GetString() { return m_String; }

protected:

	void UpdateBlinkerColor();
	void UpdateBlinkerPos();

	// Inherited via IDrawable
	void Draw(sf::RenderWindow& wnd) const override;

	// Inherited via IUpdatable
	void OnUpdate(float dt) override;

	// Inherited via IEventConsumer
	void OnEvent(const sf::Event& event) override;

protected:

	sf::RectangleShape m_Background;
	sf::RectangleShape m_Blinker;
	sf::Text m_Text;
	sf::String m_String;
	float m_BlinkingTimer;
	bool m_Editing;
	bool m_BlinkOn;
};

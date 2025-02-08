#pragma once
#include "UI/IDrawable.h"
#include <SFML/Graphics.hpp>

enum class TextAlignment : char
{
	None = 0,

	VTop = 1 << 0,
	VBottom = 1 << 1,
	VCenter = 1 << 2,
	HLeft = 1 << 3,
	HRight = 1 << 4,
	HCenter = 1 << 5,

	// Combinaisons

	TopLeft = VTop | HLeft,
	TopCenter = VTop | HCenter,
	TopRight = VTop | HRight,
	CenterLeft = VCenter | HLeft,
	Center = VCenter | HCenter,
	CenterRight = VCenter | HRight,
	BottomLeft = VBottom | HLeft,
	BottomCenter = VBottom | HCenter,
	BottomRight = VBottom | HRight
};

class Text : public IDrawable
{
public:

	Text(const sf::Font& font);
	
	void SetPosition(sf::Vector2f position) { m_Text.setPosition(position); }
	void SetString(const sf::String& string);
	void SetAlignment(TextAlignment alignment);
	void SetColor(sf::Color color) { m_Text.setFillColor(color); }

protected:

	// Inherited via IDrawable
	void Draw(sf::RenderWindow& wnd) const override;

protected:

	sf::Text m_Text;
	TextAlignment m_Alignment;
};


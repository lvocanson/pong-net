#pragma once
#include "UI/IDrawable.h"
#include <SFML/Graphics.hpp>

inline constexpr auto TEXT_SIZE = 50u;
inline constexpr auto TEXT_COLOR = sf::Color::White;

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
	
	void SetString(const sf::String& string);
	void SetAlignment(TextAlignment alignment);

protected:

	// Inherited via IDrawable
	void Draw(sf::RenderWindow& wnd) const override;

protected:

	sf::Text m_Text;
	TextAlignment m_Alignment;
};


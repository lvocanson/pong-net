#pragma once
#include <SFML/Graphics/RenderWindow.hpp>

class IDrawable
{
public:

	virtual void Draw(sf::RenderWindow& wnd) const = 0;
};
#pragma once
#include <SFML/Graphics.hpp>

class Window;

class BaseComponent : public sf::Drawable
{
public:

	virtual void SetPosition(const sf::Vector2f& position) = 0;
	virtual sf::Vector2f GetPosition() const = 0;
	virtual sf::Vector2f GetSize() const = 0;
	void SetVisible(bool visible) { m_IsVisible = visible; }
	bool IsVisible() const { return m_IsVisible; }

	virtual void Update(float dt, Window& window) = 0;

protected:

	bool m_IsVisible = true;
};

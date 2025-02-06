#pragma once

#include "Window.h"
#include <SFML/Graphics.hpp>
#include <bitset>

class InputHandler
{
public:

	void Update();

	bool IsKeyPressed(sf::Keyboard::Key key) const;
	bool IsKeyReleased(sf::Keyboard::Key key) const;
	bool IsKeyHeld(sf::Keyboard::Key key) const;

	/// <summary>
	/// Returns whether the given mouse button is pressed.
	/// </summary>
	bool IsMouseButtonPressed(sf::Mouse::Button button) const;
	bool IsMouseButtonReleased(sf::Mouse::Button button) const;
	bool IsMouseButtonHeld(sf::Mouse::Button button) const;

	/// <summary>
	/// Returns the mouse position relative to the window.
	/// </summary>
	sf::Vector2i GetMousePosition() const { return sf::Mouse::getPosition(*Window::m_Window); }

private:

	// The keyboard's state in the current- and the previous frame
	std::bitset<sf::Keyboard::KeyCount> m_CurrentKeyState;
	std::bitset<sf::Keyboard::KeyCount> m_PreviousKeyState;

	std::bitset<sf::Mouse::ButtonCount> m_CurrentButtonState;
	std::bitset<sf::Mouse::ButtonCount> m_PreviousButtonState;
};

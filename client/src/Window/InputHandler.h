#pragma once

#include "Window.h"
#include <SFML/Graphics.hpp>
#include <bitset>

class InputHandler
{
public:
    InputHandler();
    ~InputHandler() = delete;

    void Update();

    bool IsKeyPressed(sf::Keyboard::Key key);
    bool IsKeyReleased(sf::Keyboard::Key key);
    bool IsKeyHeld(sf::Keyboard::Key key);

    /// <summary>
    /// Returns whether the given mouse button is pressed.
    /// </summary>
    bool IsMouseButtonPressed(sf::Mouse::Button button);
    bool IsMouseButtonReleased(sf::Mouse::Button button);
    bool IsMouseButtonHeld(sf::Mouse::Button button);

    /// <summary>
    /// Returns the mouse position relative to the window.
    /// </summary>
    sf::Vector2i GetMousePosition() { return sf::Mouse::getPosition(*Window::m_Window); }

private:

    const unsigned int m_KeyCount;
    const unsigned int m_MouseButtonCount;

    // The keyboard's state in the current- and the previous frame
    std::bitset<sf::Keyboard::KeyCount> m_CurrentKeyState;
    std::bitset<sf::Keyboard::KeyCount> m_PreviousKeyState;

    std::bitset<sf::Mouse::ButtonCount> m_CurrentButtonState;
    std::bitset<sf::Mouse::ButtonCount> m_PreviousButtonState;
};

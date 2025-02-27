#include "InputHandler.h"

void InputHandler::Update()
{
	for (unsigned int i = 0; i < m_CurrentKeyState.size(); i++)
	{
		// Save the keyboard's state from the previous frame
		m_PreviousKeyState[i] = m_CurrentKeyState[i];

		// And save the keyboard's state in the current frame
		m_CurrentKeyState[i] = sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(i));
	}

	for (unsigned int i = 0; i < m_CurrentButtonState.size(); i++)
	{
		// Save the mouse's state from the previous frame
		m_PreviousButtonState[i] = m_CurrentButtonState[i];

		// And save the mouse's state in the current frame
		m_CurrentButtonState[i] = sf::Mouse::isButtonPressed(static_cast<sf::Mouse::Button>(i));
	}
}

bool InputHandler::IsKeyPressed(sf::Keyboard::Key key) const
{
	return (m_CurrentKeyState[(int)key] && !m_PreviousKeyState[(int)key]);
}

bool InputHandler::IsKeyReleased(sf::Keyboard::Key key) const
{
	return (!m_CurrentKeyState[(int)key] && m_PreviousKeyState[(int)key]);
}

bool InputHandler::IsKeyHeld(sf::Keyboard::Key key) const
{
	return m_CurrentKeyState[(int)key];
}

bool InputHandler::IsMouseButtonPressed(sf::Mouse::Button button) const
{
	return (m_CurrentButtonState[(int)button] && !m_PreviousButtonState[(int)button]);
}

bool InputHandler::IsMouseButtonReleased(sf::Mouse::Button button) const
{
	return (!m_CurrentButtonState[(int)button] && m_PreviousButtonState[(int)button]);
}

bool InputHandler::IsMouseButtonHeld(sf::Mouse::Button button) const
{
	return (m_CurrentButtonState[(int)button]);
}

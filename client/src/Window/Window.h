#pragma once
#include <SFML/Graphics.hpp>
#include "Game/Pong.h"
#include <functional>

class Window
{
public:
	Window();
	~Window();

#pragma region Lifecycle

	/// <summary>
	/// Creates (or recreates) a window with the given title and dimensions.
	/// </summary>
	void Create(const char* title, sf::Vector2u);
	/// <summary>
	/// Polls events and calls callbacks.
	/// </summary>
	void PollEvents();
	/// <summary>
	/// Returns whether the window is open.
	/// </summary>
	bool IsOpen() const { return m_windowRenderer->isOpen(); }
	/// <summary>
	/// Close the window.
	/// </summary>
	void Close() const { return m_windowRenderer->close(); }
	/// <summary>
	/// Returns if the window is focused.
	/// </summary>
	bool const IsFocused() const { return m_IsFocused; }
	/// <summary>
	/// Returns message in window
	/// </summary>
	const sf::Event::TextEntered& GetMessageEntered() const { return m_message; }
	/// <summary>
	/// Returns window renderer
	/// </summary>
	sf::RenderWindow& GetWindowRenderer() { return *m_windowRenderer; }

#pragma endregion

#pragma region Rendering

	/// <summary>
	/// Clears the window and render all the drawable objects.
	/// </summary>
	void Render();
	/// <summary>
	/// Sets the clear color of the window.
	/// </summary>
	void SetClearColor(const sf::Color& color) { m_ClearColor = color; }
	/// <summary>
	/// Registers a drawable object to be rendered.
	/// </summary>
	void RegisterDrawable(const sf::Drawable& drawable) { m_Drawables.push_back(&drawable); }
	/// <summary>
	/// Unregisters a drawable object.
	/// </summary>
	void UnregisterDrawable(const sf::Drawable& drawable) { m_Drawables.erase(std::remove(m_Drawables.begin(), m_Drawables.end(), &drawable), m_Drawables.end()); }
	/// <summary>
	/// Returns all the registered drawable objects.
	/// </summary>
	const std::vector<const sf::Drawable*>& GetDrawables() const { return m_Drawables; }

	void ClearAllDrawables();

#pragma endregion

#pragma region Other

	sf::Vector2f GetCenter() const { return sf::Vector2f(m_windowRenderer->getSize().x * 0.5f, m_windowRenderer->getSize().y * 0.5f); }
	float GetWidth() const { return m_windowRenderer->getSize().x; }
	float GetHeight() const { return m_windowRenderer->getSize().y; }

#pragma endregion

private:

	friend class InputHandler;

	bool m_IsFocused;
	sf::RenderWindow* m_windowRenderer;
	sf::Event::TextEntered m_message;
	sf::Color m_ClearColor;
	std::vector<const sf::Drawable*> m_Drawables;

};

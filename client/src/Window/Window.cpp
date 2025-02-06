#include "Window.h"
#include <functional>

sf::RenderWindow* Window::m_Window = nullptr;
bool Window::m_IsFocused = true;

Window::Window()
	: m_ClearColor(sf::Color::Color(51, 56, 63)), m_Drawables()
{
	m_Window = new sf::RenderWindow();
}

Window::~Window()
{
	if (m_Window->isOpen())
		m_Window->close();
	delete m_Window;
}

void Window::Create(const char* title, sf::Vector2u size)
{
	m_Window->create(sf::VideoMode(size), title);
	m_Window->setVerticalSyncEnabled(true);
	m_Window->setFramerateLimit(60);
	m_Window->setMouseCursorVisible(true);
}

void Window::PollEvents()
{
	while (const std::optional event = m_Window->pollEvent())
	{
		if (event->is<sf::Event::Closed>())
			m_Window->close();

		if (auto* keyEvent = event->getIf<sf::Event::KeyPressed>())
		{
			//onKeyPressed(keyEvent->code);
		}

		if (auto* keyEvent = event->getIf<sf::Event::KeyReleased>())
		{
			//onKeyReleased(keyEvent->code);
		}
	}
}

void Window::Render()
{
	m_Window->clear(m_ClearColor);
	for (int i = 0; i < m_Drawables.size(); i++)
	{
		// Render next frame
		if (i > m_Drawables.size()) break;

		m_Window->draw(*m_Drawables[i]);
	}

	m_Window->display();
}

void Window::ClearAllDrawables()
{
	for (auto drawable : m_Drawables)
	{
		delete drawable;
	}

	m_Drawables.clear();
}

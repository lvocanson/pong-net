#include "Window.h"

Window::Window()
	: m_ClearColor(sf::Color::Color(51, 56, 63)), m_Drawables()
{
	m_windowRenderer = new sf::RenderWindow();
}

Window::~Window()
{
	if (m_windowRenderer->isOpen())
		m_windowRenderer->close();
	delete m_windowRenderer;
}

void Window::Create(const char* title, sf::Vector2u size)
{
	m_windowRenderer->create(sf::VideoMode(size), title);
	m_windowRenderer->setVerticalSyncEnabled(true);
	m_windowRenderer->setFramerateLimit(60);
	m_windowRenderer->setMouseCursorVisible(true);
}

void Window::PollEvents()
{
	m_IsFocused = false;
	while (const std::optional event = m_windowRenderer->pollEvent())
	{
		if (event->is<sf::Event::Closed>())
			m_windowRenderer->close();

		if (auto* keyEvent = event->getIf<sf::Event::KeyPressed>())
		{
			//onKeyPressed(keyEvent->code);
		}

		if (auto* keyEvent = event->getIf<sf::Event::KeyReleased>())
		{
			//onKeyReleased(keyEvent->code);
		}

		if (auto* message = event->getIf<sf::Event::TextEntered>())
		{
			//onKeyReleased(keyEvent->code);
			m_message = *message;
			m_IsFocused = true;
		}
	}
}

void Window::Render()
{
	m_windowRenderer->clear(m_ClearColor);
	for (int i = 0; i < m_Drawables.size(); i++)
	{
		// Render next frame
		if (i > m_Drawables.size()) break;

		m_windowRenderer->draw(*m_Drawables[i]);
	}

	m_windowRenderer->display();
}

void Window::ClearAllDrawables()
{
	for (auto drawable : m_Drawables)
	{
		delete drawable;
	}

	m_Drawables.clear();
}

#pragma once
#include "Game/Pong.h"
#include "PongDisplay.h"
#include "Utils/Timer.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class App
{
public:

	App();
	int Run();

private:

	void PollEvents();
	void Update(float dt);
	void Display();

private:

	sf::RenderWindow m_Window;
	sf::Font m_Font;
	sf::Music m_Music;

	Pong m_PongGame;
	PongDisplay m_PongDisplay;
	unsigned m_LeftScore, m_RightScore;

	Timer m_Timer;
};

#pragma once
#include <SFML/Graphics.hpp>

struct Pong;
class Window;

class PongDisplay
{
public:

	PongDisplay(sf::Font&);

	void SetScore(unsigned left, unsigned right);
	void Update(const Pong& pong);
	void RegisterDrawables(Window* window);

private:

	sf::CircleShape m_Ball;
	sf::RectangleShape m_LeftPaddle;
	sf::RectangleShape m_RightPaddle;
	sf::Text m_ScoreText;
};

#include "PongDisplay.h"

#include "game/Pong.h"
#include <Window/Window.h>

PongDisplay::PongDisplay(const sf::Font& font)
	: m_Ball(BallRadius)
	, m_LeftPaddle({PaddleDistFromBorders, PaddleHeight})
	, m_RightPaddle({PaddleDistFromBorders, PaddleHeight})
	, m_ScoreText(font, "", 50u)
{
	m_Ball.setOrigin({BallRadius, BallRadius});
	m_ScoreText.setFillColor(sf::Color(0xFFFFFF66));
	m_ScoreText.setPosition({GameSizeX / 2.f, GameSizeY / 2.f});
}

void PongDisplay::SetScore(unsigned left, unsigned right)
{
	m_ScoreText.setString(std::format("{} - {}", left, right));
	m_ScoreText.setOrigin(m_ScoreText.getLocalBounds().getCenter());
}

void PongDisplay::Update(const Pong& pong)
{
	m_Ball.setPosition({pong.BallX, pong.BallY});
	m_LeftPaddle.setPosition({0.f, pong.LeftPaddle});
	m_RightPaddle.setPosition({GameSizeX - PaddleDistFromBorders, pong.RightPaddle});
}

void PongDisplay::RegisterDrawables(Window& window)
{
	window.RegisterDrawable(m_ScoreText);
	window.RegisterDrawable(m_Ball);
	window.RegisterDrawable(m_LeftPaddle);
	window.RegisterDrawable(m_RightPaddle);
}
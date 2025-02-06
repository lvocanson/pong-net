#include "GameState.h"

#include "Window/Window.h"
#include "Window/InputHandler.h"

GameState::GameState(const sf::Font& font)
	: m_PongDisplay(font)
{
}

void GameState::OnEnter(ClientApp& app)
{
	m_PongDisplay.RegisterDrawables(app.GetWindow());
	Pong& pong = app.GetPongGame();
	pong.Reset();
}

void GameState::OnUpdate(ClientApp& app, float deltaTime)
{
	PollEvents(app);
	Pong& pong = app.GetPongGame();

	pong.Update(deltaTime);
	m_PongDisplay.Update(pong);

	switch (pong.GameStateInfos())
	{
	case GameStateInfo::LeftWins:
	{
		++app.m_LeftScore;
		break;
	}
	case GameStateInfo::RightWins:
	{
		++app.m_RightScore;
		break;
	}
	default: return;
	}

	pong.Reset();

	m_PongDisplay.SetScore(app.m_LeftScore, app.m_RightScore);
}

void GameState::OnExit(ClientApp& app)
{
}

void GameState::PollEvents(ClientApp& app)
{
	auto& input = app.GetInputHandler();
	Pong& pong = app.GetPongGame();

	// PRESSED

	if (input.IsKeyPressed(sf::Keyboard::Key::W) || input.IsKeyPressed(sf::Keyboard::Key::Z))
	{
		pong.Behaviours |= PaddlesBehaviour::LeftUp;
	}

	if (input.IsKeyPressed(sf::Keyboard::Key::S))
	{
		pong.Behaviours |= PaddlesBehaviour::LeftDown;
	}

	if (input.IsKeyPressed(sf::Keyboard::Key::Up))
	{
		pong.Behaviours |= PaddlesBehaviour::RightUp;
	}

	if (input.IsKeyPressed(sf::Keyboard::Key::Down))
	{
		pong.Behaviours |= PaddlesBehaviour::RightDown;
	}

	// RELEASED

	if (input.IsKeyReleased(sf::Keyboard::Key::W) || input.IsKeyReleased(sf::Keyboard::Key::Z))
	{
		pong.Behaviours &= ~PaddlesBehaviour::LeftUp;
	}

	if (input.IsKeyReleased(sf::Keyboard::Key::S))
	{
		pong.Behaviours &= ~PaddlesBehaviour::LeftDown;
	}

	if (input.IsKeyReleased(sf::Keyboard::Key::Up))
	{
		pong.Behaviours &= ~PaddlesBehaviour::RightUp;
	}

	if (input.IsKeyReleased(sf::Keyboard::Key::Down))
	{
		pong.Behaviours &= ~PaddlesBehaviour::RightDown;
	}
}
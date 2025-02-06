#include "GameState.h"

#include "Window/Window.h"
#include "Window/InputHandler.h"

#pragma region  Constructor

GameState::GameState()
	: m_PongDisplay(nullptr)
{
}

GameState::~GameState()
{
}

#pragma endregion

#pragma region  Override

void GameState::OnEnter(ClientApp& app)
{
	m_PongDisplay = new PongDisplay(*app.GetFontByName("JuliaMono-Regular.ttf"));
	m_PongDisplay->RegisterDrawables(app.GetWindow());
}

void GameState::OnUpdate(ClientApp& app, float deltaTime)
{	
	PollEvents(app);
	Pong pong = app.GetPongGame();

	pong.Update(deltaTime);
	m_PongDisplay->Update(pong);

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
	m_PongDisplay->SetScore(app.m_LeftScore, app.m_RightScore);
}

void GameState::OnExit(ClientApp& app)
{
}

#pragma endregion


void GameState::PollEvents(ClientApp& app)
{
	InputHandler* input = app.GetInputHandler();
	Pong pong = app.GetPongGame();

	// PRESSED

	if (input->IsKeyPressed(sf::Keyboard::Key::W) || input->IsKeyPressed(sf::Keyboard::Key::Z))
	{
		pong.Behaviours |= PaddlesBehaviour::LeftUp;
	}	
	
	if (input->IsKeyPressed(sf::Keyboard::Key::S))
	{
		pong.Behaviours |= PaddlesBehaviour::LeftDown;
	}	

	if (input->IsKeyPressed(sf::Keyboard::Key::Up))
	{
		pong.Behaviours |= PaddlesBehaviour::RightUp;
	}	

	if (input->IsKeyPressed(sf::Keyboard::Key::Down))
	{
		pong.Behaviours |= PaddlesBehaviour::RightDown;
	}	

	// RELEASED

	if (input->IsKeyReleased(sf::Keyboard::Key::W) || input->IsKeyReleased(sf::Keyboard::Key::Z))
	{
		pong.Behaviours &= ~PaddlesBehaviour::LeftUp;
	}

	if (input->IsKeyReleased(sf::Keyboard::Key::S))
	{
		pong.Behaviours &= ~PaddlesBehaviour::LeftDown;
	}

	if (input->IsKeyReleased(sf::Keyboard::Key::Up))
	{
		pong.Behaviours &= ~PaddlesBehaviour::RightUp;
	}

	if (input->IsKeyReleased(sf::Keyboard::Key::Down))
	{
		pong.Behaviours &= ~PaddlesBehaviour::RightDown;
	}
}
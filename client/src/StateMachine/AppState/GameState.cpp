#include "GameState.h"

#include "Window/Window.h"
#include "Window/InputHandler.h"

#pragma region  Constructor

GameState::GameState()
	: m_PongGame()
	, m_PongDisplay(nullptr)
	, m_LeftScore(0)
	, m_RightScore(0)
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

	m_PongGame.Update(deltaTime);
	m_PongDisplay->Update(m_PongGame);

	switch (m_PongGame.GetGameState())
	{
	case GameStateInfos::LeftWins:
	{
		++m_LeftScore;
		break;
	}
	case GameStateInfos::RightWins:
	{
		++m_RightScore;
		break;
	}
	default: return;
	}

	m_PongGame.Reset();
	m_PongDisplay->SetScore(m_LeftScore, m_RightScore);
}

void GameState::OnExit(ClientApp& app)
{
}

#pragma endregion


void GameState::PollEvents(ClientApp& app)
{
	InputHandler* input = app.GetInputHandler();

	// PRESSED

	if (input->IsKeyPressed(sf::Keyboard::Key::W) || input->IsKeyPressed(sf::Keyboard::Key::Z))
	{
		m_PongGame.Behaviours |= PaddlesBehaviour::LeftUp;
	}	
	
	if (input->IsKeyPressed(sf::Keyboard::Key::S))
	{
		m_PongGame.Behaviours |= PaddlesBehaviour::LeftDown;
	}	

	if (input->IsKeyPressed(sf::Keyboard::Key::Up))
	{
		m_PongGame.Behaviours |= PaddlesBehaviour::RightUp;
	}	

	if (input->IsKeyPressed(sf::Keyboard::Key::Down))
	{
		m_PongGame.Behaviours |= PaddlesBehaviour::RightDown;
	}	

	// RELEASED

	if (input->IsKeyReleased(sf::Keyboard::Key::W) || input->IsKeyReleased(sf::Keyboard::Key::Z))
	{
		m_PongGame.Behaviours &= ~PaddlesBehaviour::LeftUp;
	}

	if (input->IsKeyReleased(sf::Keyboard::Key::S))
	{
		m_PongGame.Behaviours &= ~PaddlesBehaviour::LeftDown;
	}

	if (input->IsKeyReleased(sf::Keyboard::Key::Up))
	{
		m_PongGame.Behaviours &= ~PaddlesBehaviour::RightUp;
	}

	if (input->IsKeyReleased(sf::Keyboard::Key::Down))
	{
		m_PongGame.Behaviours &= ~PaddlesBehaviour::RightDown;
	}
}
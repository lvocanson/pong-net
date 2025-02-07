#include "GameState.h"

#include "Window/Window.h"
#include "Window/InputHandler.h"

GameState::GameState(const sf::Font& font, const Message_RoomJoinResponse::PlayerSide& side)
	: m_PongDisplay(font)
	, currentSide(side)
	, m_LoadingText(font)
	, m_LoadingTriangle(15.f, 15)
	, m_LoadingCircle(15.f, 15)
{
	InitLoading();
}

void GameState::OnEnter(ClientApp& app)
{
	m_PongDisplay.RegisterDrawables(app.GetWindow());
	app.GetWindow().RegisterDrawable(m_LoadingTriangle);
	app.GetWindow().RegisterDrawable(m_LoadingCircle);
	app.GetWindow().RegisterDrawable(m_LoadingText);
}

void GameState::OnUpdate(ClientApp& app, float deltaTime)
{
	PollEvents(app);
	m_PongDisplay.Update(app.GetPongGame());

	auto [leftScore, rightScore] = app.GetScores();
	m_PongDisplay.SetScore(leftScore, rightScore);

	if (app.GetPlayingState() == PlayingState::Paused)
	{
		m_LoadingRotation += 90.f * deltaTime;
		m_LoadingTriangle.setRotation(sf::degrees(m_LoadingRotation));
		m_LoadingCircle.setRotation(sf::degrees(m_LoadingRotation));
	}
	else 
	{
		app.GetWindow().UnregisterDrawable(m_LoadingTriangle);
		app.GetWindow().UnregisterDrawable(m_LoadingCircle);
		app.GetWindow().UnregisterDrawable(m_LoadingText);
	}
}

void GameState::OnExit(ClientApp& app)
{
	m_PongDisplay.UnregisterDrawable(app.GetWindow());
	app.GetWindow().UnregisterDrawable(m_LoadingTriangle);
	app.GetWindow().UnregisterDrawable(m_LoadingCircle);
	app.GetWindow().UnregisterDrawable(m_LoadingText);
}

void GameState::PollEvents(ClientApp& app)
{
	auto& input = app.GetInputHandler();
	Pong& pong = app.GetPongGame();

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


    //bool moveUp = input.IsKeyPressed(sf::Keyboard::Key::W) || input.IsKeyPressed(sf::Keyboard::Key::Z) || input.IsKeyPressed(sf::Keyboard::Key::Up);
    //bool moveDown = input.IsKeyPressed(sf::Keyboard::Key::S) || input.IsKeyPressed(sf::Keyboard::Key::Down);

    //bool releaseUp = input.IsKeyReleased(sf::Keyboard::Key::W) || input.IsKeyReleased(sf::Keyboard::Key::Z) || input.IsKeyReleased(sf::Keyboard::Key::Up);
    //bool releaseDown = input.IsKeyReleased(sf::Keyboard::Key::S) || input.IsKeyReleased(sf::Keyboard::Key::Down);

    //if (pong.isLeftPlayer) 
    //{
    //    if (moveUp) pong.Behaviours |= PaddlesBehaviour::LeftUp;
    //    if (moveDown) pong.Behaviours |= PaddlesBehaviour::LeftDown;
    //    if (releaseUp) pong.Behaviours &= ~PaddlesBehaviour::LeftUp;
    //    if (releaseDown) pong.Behaviours &= ~PaddlesBehaviour::LeftDown;
    //}
    //else 
    //{
    //    if (moveUp) pong.Behaviours |= PaddlesBehaviour::RightUp;
    //    if (moveDown) pong.Behaviours |= PaddlesBehaviour::RightDown;
    //    if (releaseUp) pong.Behaviours &= ~PaddlesBehaviour::RightUp;
    //    if (releaseDown) pong.Behaviours &= ~PaddlesBehaviour::RightDown;
    //}
}

void GameState::InitLoading()
{
	m_LoadingTriangle.setFillColor(sf::Color::Transparent);
	m_LoadingTriangle.setOutlineColor(sf::Color::Blue);
	m_LoadingTriangle.setOutlineThickness(3.f);
	m_LoadingTriangle.setPosition(sf::Vector2f(GameSizeX * 0.5f, 25.f));
	m_LoadingTriangle.setOrigin(m_LoadingTriangle.getLocalBounds().getCenter());
	m_LoadingTriangle.setPointCount(4);

	m_LoadingCircle.setFillColor(sf::Color::Transparent);
	m_LoadingCircle.setOutlineColor(sf::Color::Magenta);
	m_LoadingCircle.setOutlineThickness(3.f);
	m_LoadingCircle.setPosition(sf::Vector2f(GameSizeX * 0.5f, 25.f));
	m_LoadingCircle.setOrigin(m_LoadingCircle.getLocalBounds().getCenter());

	m_LoadingText.SetPosition(sf::Vector2f(GameSizeX * 0.25f, 25.f));
	m_LoadingText.SetColor(sf::Color(0xFFFFFF66));
	m_LoadingText.SetText("Wainting for player...");
}
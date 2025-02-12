#include "GameScene.h"
#include "ClientApp.h"
#include "Game/Pong.h"
#include "UI/UIHelper.h"
#include "Utils/EnumOperators.h"

GameScene::GameScene(ClientApp& app, const sf::Font& font, sf::Vector2u screenSize)
	: m_App(app)
	, m_Ball(BallRadius)
	, m_LeftPaddle({PaddleDistFromBorders, PaddleHeight})
	, m_RightPaddle({PaddleDistFromBorders, PaddleHeight})
	, m_ScoreTxt(font)
	, m_StatusTxt(font)
{
	m_ScoreTxt.SetAlignment(TextAlignment::Center);
	m_StatusTxt.SetAlignment(TextAlignment::TopCenter);

	Enable(m_ScoreTxt, m_StatusTxt);
	OnWindowResized(screenSize);
}

void GameScene::Draw(sf::RenderWindow& wnd) const
{
	wnd.draw(m_RightPaddle);
	wnd.draw(m_LeftPaddle);
	wnd.draw(m_Ball);

	Scene::Draw(wnd);
}

void GameScene::OnUpdate(float dt)
{
	const auto& game = m_App.GetPongGame();

	m_Ball.setPosition({game.BallX * m_SizeMultiplier.x, game.BallY * m_SizeMultiplier.y});
	m_LeftPaddle.setPosition({0.f, game.LeftPaddle * m_SizeMultiplier.y});
	m_RightPaddle.setPosition({(GameSizeX - PaddleDistFromBorders) * m_SizeMultiplier.x, game.RightPaddle * m_SizeMultiplier.y});

	auto [leftScore, rightScore] = m_App.GetScores();
	m_ScoreTxt.SetString(std::format("{} - {}", leftScore, rightScore));

	auto state = m_App.GetPlayingState();
	if (state == PlayingState::Playing)
	{
		m_StatusTxt.SetString("");
	}
	else
	{
		m_StatusTxt.SetString("PAUSED");
	}
}

void GameScene::OnEvent(const sf::Event& event)
{
	auto& game = m_App.GetPongGame();

	if (auto* e = event.getIf<sf::Event::KeyPressed>())
	{
		using enum sf::Keyboard::Key;
		switch (e->code)
		{
		case W:
		case Z:
			game.Behaviours |= PaddlesBehaviour::LeftUp;
			break;
		case S:
			game.Behaviours |= PaddlesBehaviour::LeftDown;
			break;
		case Up:
			game.Behaviours |= PaddlesBehaviour::RightUp;
			break;
		case Down:
			game.Behaviours |= PaddlesBehaviour::RightDown;
			break;
		}
	}

	else if (auto* e = event.getIf<sf::Event::KeyReleased>())
	{
		using enum sf::Keyboard::Key;
		switch (e->code)
		{
		case W:
		case Z:
			game.Behaviours &= ~PaddlesBehaviour::LeftUp;
			break;
		case S:
			game.Behaviours &= ~PaddlesBehaviour::LeftDown;
			break;
		case Up:
			game.Behaviours &= ~PaddlesBehaviour::RightUp;
			break;
		case Down:
			game.Behaviours &= ~PaddlesBehaviour::RightDown;
			break;
		}
	}
}

void GameScene::OnWindowResized(sf::Vector2u screenSize)
{
	auto screenSizeF = sf::Vector2f(screenSize);
	m_SizeMultiplier = {screenSizeF.x / GameSizeX, screenSizeF.y / GameSizeY};

	using namespace UIHelper;

	auto screenCenter = screenSizeF / 2.f;
	m_ScoreTxt.SetPosition(screenCenter);
	m_StatusTxt.SetPosition({screenCenter.x, ElementMargin});
}

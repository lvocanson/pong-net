#pragma once
#include "UI/Scene.h"
#include "UI/Stuff/Text.h"

class GameScene : public Scene
{
public:

	GameScene(ClientApp& app, const sf::Font& font, sf::Vector2u screenSize);

private:

	void Draw(sf::RenderWindow& wnd) const override;
	void OnUpdate(float dt) override;
	void OnEvent(const sf::Event& event) override;
	void OnWindowResized(sf::Vector2u screenSize);

private:
	
	ClientApp& m_App;
	sf::Vector2f m_SizeMultiplier;

	sf::CircleShape m_Ball;
	sf::RectangleShape m_LeftPaddle;
	sf::RectangleShape m_RightPaddle;
	Text m_ScoreTxt;
	Text m_StatusTxt;
};

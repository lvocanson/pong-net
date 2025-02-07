#pragma once
#include <ClientApp.h>
#include "Game/Pong.h"
#include <Components/TextComponent.h>

class GameState : public State<ClientApp>
{
public:

	GameState(const sf::Font& font, const Message_RoomJoinResponse::PlayerSide& side);

	void OnEnter(ClientApp& app) override;
	void OnUpdate(ClientApp& app, float deltaTime) override;
	void OnExit(ClientApp& app) override;

	void PollEvents(ClientApp& app);
	void InitLoading();

private:

	PongDisplay m_PongDisplay;
	TextComponent m_LoadingText;
	sf::CircleShape m_LoadingTriangle;
	sf::CircleShape m_LoadingCircle;
	float m_LoadingRotation = 0.0f;
	Message_RoomJoinResponse::PlayerSide currentSide;
};


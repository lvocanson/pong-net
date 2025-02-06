#pragma once
#include <ClientApp.h>
#include "Game/Pong.h"

class GameState : public State<ClientApp>
{
public:

	GameState(const sf::Font& font);

	void OnEnter(ClientApp& app) override;
	void OnUpdate(ClientApp& app, float deltaTime) override;
	void OnExit(ClientApp& app) override;

	void PollEvents(ClientApp& app);

private:

	PongDisplay m_PongDisplay;
};


#pragma once
/*
#include <ClientApp.h>
#include "StateMachine/StateMachine.h"
#include <Components/ButtonComponent.h>

class LobbyState : public State<ClientApp>
{
public:
	LobbyState(ClientApp& app);

	virtual void OnEnter(ClientApp& app) override;
	virtual void OnUpdate(ClientApp& app, float deltaTime) override;
	virtual void OnExit(ClientApp& app) override;

	void InitReturnButton(const sf::Vector2f& pos);
	void InitCreateButton(const sf::Vector2f& pos);
	void JoinLobbyRequest(int lobbyID);

private:

	ClientApp& m_ClientApp;

	int m_CurrentLobbyID;
	bool m_IsLobbyInit = false;
	bool m_IsInLobby = false;
	bool m_IsTryingToJoinLobby = false;

	NammedButton m_ReturnButton;
	NammedButton m_CreateButton;

	std::string m_LobbyGameMode;
	std::vector<std::tuple<uint16_t, NammedButton>> m_LobbyBtns;
};
*/

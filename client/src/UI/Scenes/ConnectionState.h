#pragma once
/*
#include "ClientApp.h"
#include "StateMachine/StateMachine.h"
#include "Components/ButtonComponent.h"
#include "Components/InsertFieldComponent.h"

class ConnectionState : public State<ClientApp>
{
public:

	ConnectionState(ClientApp& app);

	void OnEnter(ClientApp& derived) override;
	void OnUpdate(ClientApp& derived, float deltaTime) override;
	void OnExit(ClientApp& derived) override;
	void InitBackButton(const sf::Vector2f& pos);
	void InitLobbyButton(const sf::Vector2f& pos);
	void InitQuickGameButton(const sf::Vector2f& pos);

private:

	ClientApp& m_ClientApp;

	NammedButton m_BackBtn;
	NammedButton m_ShowLobbyBtn;
	NammedButton m_QuickGameBtn;

	std::function<void()> m_executeFunction;
	bool m_callFunction;
};
*/

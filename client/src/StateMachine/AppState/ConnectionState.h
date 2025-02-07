#pragma once

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

	void InitIpField(const sf::Vector2f& pos);
	void InitUsernameField(const sf::Vector2f& pos);
	void InitBackButton(const sf::Vector2f& pos);
	void InitLobbyButton(const sf::Vector2f& pos);
	void InitQuickGameButton(const sf::Vector2f& pos);
	void ShowLobbyFunction();

private:

	bool m_IsTryingToConnect = false;

	ClientApp& m_ClientApp;

	ButtonComponent m_BackBtn;
	ButtonComponent m_ShowLobbyBtn;
	ButtonComponent m_QuickGameBtn;

	InsertFieldComponent m_IpField;
	InsertFieldComponent m_UsernameField;

	std::string m_Name = "";

	std::function<void()> m_executeFunction;
	bool m_callFunction;
};

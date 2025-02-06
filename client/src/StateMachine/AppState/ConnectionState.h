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
	void InitConnectButton(const sf::Vector2f& pos);

	void ActiveButtonFunction(ClientApp& app);

private:

	bool m_IsTryingToConnect = false;

	ClientApp& m_ClientApp;

	ButtonComponent m_BackBtn;
	ButtonComponent m_ConnectBtn;

	InsertFieldComponent m_IpField;
	InsertFieldComponent m_UsernameField;

	ButtonFunction m_CurrentFunction;

	std::string m_Name = "";
};

#pragma once
#include "ClientApp.h"
#include "StateMachine/StateMachine.h"
#include "Components/ButtonComponent.h"
#include "Components/InsertFieldComponent.h"
#include "Components/SliderComponent.h"
#include "Window/Window.h"
#include <SFML/Graphics.hpp>

class MenuState : public State<ClientApp>
{
public:

	MenuState(ClientApp& app);

	virtual void OnEnter(ClientApp& app) override;
	virtual void OnUpdate(ClientApp& app, float deltaTime) override;
	virtual void OnExit(ClientApp& app) override;	

	void InitIpField(const sf::Vector2f& pos);
	void InitUsernameField(const sf::Vector2f& pos);
	void InitSlider(sf::Vector2f& pos, float width, float minValue, float maxValue, const sf::Font& font);
	void InitConnectionButton(const sf::Vector2f& pos);
	void InitDisconnectButton(const sf::Vector2f& pos);
	void InitQuitButton(const sf::Vector2f& pos);

	void ShowLobbyFunction();

private:

	ClientApp& m_ClientApp;
	ButtonComponent m_ConnectBtn;
	ButtonComponent m_DisconnectBtn;
	ButtonComponent m_QuitBtn;

	SliderComponent m_Slider;
	TextComponent m_SliderText;

	InsertFieldComponent m_IpField;
	InsertFieldComponent m_UsernameField;

	std::function<void()> m_executeFunction;
	bool m_callFunction;

	std::string m_Name = "";
};
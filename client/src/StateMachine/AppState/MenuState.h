#pragma once
#include "ClientApp.h"
#include "StateMachine/StateMachine.h"
#include "Components/ButtonComponent.h"
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

	void InitSlider(sf::Vector2f& pos, float width, float minValue, float maxValue, const sf::Font& font);
	void InitPlayButton(const sf::Vector2f& pos);
	void InitConnectionButton(const sf::Vector2f& pos);
	void InitDisconnectButton(const sf::Vector2f& pos);
	void InitQuitButton(const sf::Vector2f& pos);

private:

	ClientApp& m_ClientApp;

	ButtonComponent m_PlayBtn;
	ButtonComponent m_ConnectBtn;
	ButtonComponent m_DisconnectBtn;
	ButtonComponent m_QuitBtn;

	SliderComponent m_Slider;
	TextComponent m_SliderText;

	std::function<void()> m_executeFunction;
	bool m_callFunction;
};
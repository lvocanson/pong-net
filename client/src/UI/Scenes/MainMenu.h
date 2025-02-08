#pragma once
#include "UI/Scene.h"
#include "UI/Stuff/Button.h"
#include "UI/Stuff/Text.h"
#include "UI/Stuff/TextField.h"

class MainMenu : public Scene
{
public:

	MainMenu(ClientApp& app, const sf::Font& font, sf::Vector2u screenSize);

private:

	void OnEvent(const sf::Event& event) override;
	void OnWindowResized(sf::Vector2u screenSize);
	void OnConnectClicked();

private:

	ClientApp& m_App;
	Text m_TitleTxt, m_ConnectTxt, m_QuitTxt;
	Button m_ConnectBtn, m_QuitBtn;
	TextField m_AddressFld;
};
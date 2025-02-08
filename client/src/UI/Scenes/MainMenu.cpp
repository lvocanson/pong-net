#include "MainMenu.h"
#include "ClientApp.h"
#include "UI/UIHelper.h"

MainMenu::MainMenu(ClientApp& app, const sf::Font& font, sf::Vector2u screenSize)
	: m_App(app)
	, m_TitleTxt(font)
	, m_ConnectTxt(font)
	, m_QuitTxt(font)
	, m_AddressFld(font)
{
	m_TitleTxt.SetString("Pong");
	m_TitleTxt.SetAlignment(TextAlignment::TopCenter);

	m_ConnectBtn.SetBgColor(sf::Color::Green);
	m_ConnectBtn.SetCallback(std::bind(&MainMenu::OnConnectClicked, this));

	m_QuitBtn.SetBgColor(sf::Color::Red);
	m_QuitBtn.SetCallback(std::bind(&ClientApp::Quit, &m_App));

	m_ConnectTxt.SetString("Connect");
	m_ConnectTxt.SetAlignment(TextAlignment::Center);

	m_QuitTxt.SetString("Quit");
	m_QuitTxt.SetAlignment(TextAlignment::Center);

	auto localPhrase = IpAddress::LocalAddress().ToPhrase();
	auto string = sf::String(std::string(localPhrase.View()));
	m_AddressFld.SetString(string);

	Enable(m_ConnectBtn, m_QuitBtn, m_TitleTxt, m_ConnectTxt, m_QuitTxt, m_AddressFld);
	OnWindowResized(screenSize);
}

void MainMenu::OnEvent(const sf::Event& event)
{
	if (auto* e = event.getIf<sf::Event::Resized>())
	{
		OnWindowResized(e->size);
	}

	Scene::OnEvent(event);
}

void MainMenu::OnWindowResized(sf::Vector2u screenSize)
{
	using namespace UIHelper;
	sf::Vector2f center = sf::Vector2f(screenSize) / 2.f;
	m_TitleTxt.SetPosition({center.x, ElementMargin});

	m_AddressFld.SetRect({center, FldSize});

	float btnPosXFromCenter = (BtnSize.x + 5.f) / 2.f;
	float btnPosY = static_cast<float>(screenSize.y) - BtnSize.y / 2.f - 5.f;
	sf::Vector2f connectBtnPos = {center.x + btnPosXFromCenter, btnPosY};
	sf::Vector2f quitBtnPos = {center.x - btnPosXFromCenter, btnPosY};
	m_ConnectBtn.SetRect({connectBtnPos, BtnSize});
	m_ConnectTxt.SetPosition(connectBtnPos);
	m_QuitBtn.SetRect({quitBtnPos, BtnSize});
	m_QuitTxt.SetPosition(quitBtnPos);
}

void MainMenu::OnConnectClicked()
{
	IpPhrase phrase = {m_AddressFld.GetString().toAnsiString().c_str()};
	m_App.ConnectToServer(phrase);
}

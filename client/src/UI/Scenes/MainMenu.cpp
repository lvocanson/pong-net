#include "MainMenu.h"
#include "ClientApp.h"
#include "UI/UIHelper.h"

MainMenu::MainMenu(ClientApp& app, const sf::Font& font, sf::Vector2u screenSize)
	: m_App(app)
	, m_TitleTxt(font)
	, m_ConnectTxt(font)
	, m_QuitTxt(font)
	, m_AddressFld(font)
	, m_VolumeTxt(font)
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

	m_VolumeTxt.SetString("Volume");
	m_VolumeTxt.SetAlignment(TextAlignment::BottomCenter);

	m_VolumeSldr.SetLimits(0.f, 100.f); // sf::Music volume is between 0 and 100
	m_VolumeSldr.SetKnobRadius(UIHelper::SldrSize.y);
	m_VolumeSldr.SetValue(app.GetMusicVolume());
	m_VolumeSldr.SetCallback(std::bind(&ClientApp::SetMusicVolume, &app, std::placeholders::_1));

	auto localPhrase = IpAddress::LocalAddress().ToPhrase();
	auto string = sf::String(std::string(localPhrase.View()));
	m_AddressFld.SetString(string);

	Enable(m_ConnectBtn, m_QuitBtn, m_TitleTxt, m_ConnectTxt, m_QuitTxt, m_VolumeTxt, m_AddressFld, m_VolumeSldr);
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
	auto screenSizeF = sf::Vector2f(screenSize);
	sf::Vector2f screenCenter = screenSizeF / 2.f;

	using namespace UIHelper;

	m_TitleTxt.SetPosition({screenCenter.x, ElementMargin});

	m_AddressFld.SetRect({screenCenter, FldSize});

	float btnPosXFromCenter = (BtnSize.x + 5.f) / 2.f;
	float btnPosY = static_cast<float>(screenSize.y) - BtnSize.y / 2.f - 5.f;
	sf::Vector2f connectBtnPos = {screenCenter.x + btnPosXFromCenter, btnPosY};
	sf::Vector2f quitBtnPos = {screenCenter.x - btnPosXFromCenter, btnPosY};
	m_ConnectBtn.SetRect({connectBtnPos, BtnSize});
	m_ConnectTxt.SetPosition(connectBtnPos);
	m_QuitBtn.SetRect({quitBtnPos, BtnSize});
	m_QuitTxt.SetPosition(quitBtnPos);

	sf::Vector2f volumePos = screenSizeF - SldrSize - sf::Vector2{ElementMargin + SldrSize.y, ElementMargin + SldrSize.y};
	auto volumeRect = sf::Rect{volumePos, SldrSize};
	m_VolumeSldr.SetRect(volumeRect);
	m_VolumeTxt.SetPosition(volumeRect.getCenter() - sf::Vector2{0.f, ElementMargin + SldrSize.y});
}

void MainMenu::OnConnectClicked()
{
	IpPhrase phrase = {m_AddressFld.GetString().toAnsiString().c_str()};
	m_App.ConnectToServer(phrase);
}

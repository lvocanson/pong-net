#include "ConnectionState.h"
#include "MenuState.h"
#include "GameState.h"
#include <Network/PacketWrapper.h>
#include "LobbyState.h"

constexpr float CONNECTION_TIMEOUT_TIME = 5.0f;

ConnectionState::ConnectionState(ClientApp& app)
	: m_ClientApp(app)
	, m_BackBtn(app.GetFont(), app.GetInputHandler())
	, m_ShowLobbyBtn(app.GetFont(), app.GetInputHandler())
	, m_QuickGameBtn(app.GetFont(), app.GetInputHandler())
	, m_executeFunction()
	, m_callFunction(false)
{
	sf::Vector2f sizeField = FIELD_SIZE;
	float yOffset = app.GetWindow().GetHeight() * 0.1f;
	float xMiddleScreen = app.GetWindow().GetWidth() * 0.5f;

	// Init all graphics element
	sf::Vector2f quickGameButtonPos = sf::Vector2f(xMiddleScreen, 3.f * yOffset);
	sf::Vector2f lobbyButtonPos = sf::Vector2f(xMiddleScreen, 5.f * yOffset);
	sf::Vector2f backButtonPos = sf::Vector2f(xMiddleScreen, 7.f * yOffset);

	InitBackButton(backButtonPos);
	InitLobbyButton(lobbyButtonPos);
	InitQuickGameButton(quickGameButtonPos);
}

void ConnectionState::OnEnter(ClientApp& app)
{
	app.GetWindow().RegisterDrawable(m_BackBtn);
	app.GetWindow().RegisterDrawable(m_ShowLobbyBtn);
	app.GetWindow().RegisterDrawable(m_QuickGameBtn);
}

void ConnectionState::OnUpdate(ClientApp& app, float dt)
{
	m_BackBtn.Update(dt, app.GetWindow());
	m_ShowLobbyBtn.Update(dt, app.GetWindow());
	m_QuickGameBtn.Update(dt, app.GetWindow());

	//if (m_IsTryingToConnect && app.GetConnectionStateInfo() == ConnectionStateInfos::IsConnected)
	//{
	//	//m_ClientApp.ChangeState<GameState>(m_ClientApp.GetFont());
	//}
	//else 

	if (m_ShowLobbyBtn.IsPressed())
	{
		m_ClientApp.ChangeState<LobbyState>(m_ClientApp);
		return;
	}
	else if (m_BackBtn.IsPressed()) 
	{
		m_ClientApp.ChangeState<MenuState>(m_ClientApp);
		return;
	}
	else if (m_QuickGameBtn.IsPressed())
	{
		Message request(MessageType::QuickMatchRequest);
		auto wrapper = PacketWrapper::Wrap(request);
		wrapper.Sign(m_ClientApp.GetSignature());
		wrapper.Send(m_ClientApp.GetSocket(), m_ClientApp.GetServerAddr());
		return;
	}
}

void ConnectionState::OnExit(ClientApp& app)
{
	app.GetWindow().UnregisterDrawable(m_BackBtn);
	app.GetWindow().UnregisterDrawable(m_ShowLobbyBtn);
	app.GetWindow().UnregisterDrawable(m_QuickGameBtn);
}

void ConnectionState::InitBackButton(const sf::Vector2f& pos)
{
	sf::Color OrangeRed(231, 62, 1);
	std::string btnText = "Return to menu";

	m_BackBtn.SetSize(BUTTON_SIZE_EXTRA_EXTENDED);
	m_BackBtn.SetPosition(pos);
	m_BackBtn.SetColor(OrangeRed);
	m_BackBtn.SetButtonText(btnText);
}

void ConnectionState::InitLobbyButton(const sf::Vector2f& pos)
{
	sf::Color Emerald(1, 215, 88);
	std::string btnText = "Show lobby";

	m_ShowLobbyBtn.SetSize(BUTTON_SIZE_EXTRA_EXTENDED);
	m_ShowLobbyBtn.SetPosition(pos);
	m_ShowLobbyBtn.SetColor(Emerald);
	m_ShowLobbyBtn.SetButtonText(btnText);
}

void ConnectionState::InitQuickGameButton(const sf::Vector2f& pos)
{
	sf::Color Emerald(1, 215, 88);
	std::string btnText = "Quick Connect";

	m_QuickGameBtn.SetSize(BUTTON_SIZE_EXTRA_EXTENDED);
	m_QuickGameBtn.SetPosition(pos);
	m_QuickGameBtn.SetColor(Emerald);
	m_QuickGameBtn.SetButtonText(btnText);
}

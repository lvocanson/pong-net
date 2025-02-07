#include "ConnectionState.h"
#include "MenuState.h"
#include "GameState.h"
#include <Network/PacketWrapper.h>

constexpr float CONNECTION_TIMEOUT_TIME = 5.0f;

ConnectionState::ConnectionState(ClientApp& app)
	: m_ClientApp(app)
	, m_BackBtn(app.GetFont(), app.GetInputHandler())
	, m_ShowLobbyBtn(app.GetFont(), app.GetInputHandler())
	, m_QuickGameBtn(app.GetFont(), app.GetInputHandler())
	, m_IpField(app.GetFont(), app.GetInputHandler())
	, m_UsernameField(app.GetFont(), app.GetInputHandler())
	, m_executeFunction()
	, m_callFunction(false)
{
	sf::Vector2f sizeField = FIELD_SIZE;
	float yOffset = app.GetWindow().GetHeight() * 0.1f;
	float xMiddleScreen = app.GetWindow().GetWidth() * 0.5f;

	sf::Vector2f fieldPos = sf::Vector2f(xMiddleScreen, 3.f * yOffset);

	// Init all graphics element
	sf::Vector2f quickGameButtonPos = sf::Vector2f(xMiddleScreen, 6.f * yOffset);
	sf::Vector2f lobbyButtonPos = sf::Vector2f(xMiddleScreen, 6.75f * yOffset);
	sf::Vector2f backButtonPos = sf::Vector2f(xMiddleScreen, 8.f * yOffset);

	InitIpField(fieldPos);
	fieldPos.y += 1.5f * yOffset;

	InitUsernameField(fieldPos);

	InitBackButton(backButtonPos);
	InitLobbyButton(lobbyButtonPos);
	InitQuickGameButton(quickGameButtonPos);
}

void ConnectionState::OnEnter(ClientApp& app)
{
	app.GetWindow().RegisterDrawable(m_IpField);
	app.GetWindow().RegisterDrawable(m_UsernameField);

	app.GetWindow().RegisterDrawable(m_BackBtn);
	app.GetWindow().RegisterDrawable(m_ShowLobbyBtn);
	app.GetWindow().RegisterDrawable(m_QuickGameBtn);
}

void ConnectionState::OnUpdate(ClientApp& app, float dt)
{
	m_IpField.Update(dt, app.GetWindow());
	m_UsernameField.Update(dt, app.GetWindow());
	m_BackBtn.Update(dt, app.GetWindow());
	m_ShowLobbyBtn.Update(dt, app.GetWindow());
	m_QuickGameBtn.Update(dt, app.GetWindow());

	//if (m_IsTryingToConnect && app.GetConnectionStateInfo() == ConnectionStateInfos::IsConnected)
	//{
	//	//m_ClientApp.ChangeState<GameState>(m_ClientApp.GetFont());
	//}
	//else 
	if(app.GetConnectionStateInfo() == ConnectionStateInfos::FailedConnection)
	{
		m_IsTryingToConnect = false;
	}

	if (m_ShowLobbyBtn.IsPressed())
	{
		ShowLobbyFunction();
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
	app.GetWindow().UnregisterDrawable(m_IpField);
	app.GetWindow().UnregisterDrawable(m_UsernameField);
	app.GetWindow().UnregisterDrawable(m_BackBtn);
	app.GetWindow().UnregisterDrawable(m_ShowLobbyBtn);
	app.GetWindow().UnregisterDrawable(m_QuickGameBtn);
}


void ConnectionState::InitIpField(const sf::Vector2f& pos)
{
	std::string ipLabel = "Server Phrase";
	m_IpField.SetSize(FIELD_SIZE);
	m_IpField.SetLabel(ipLabel);
	m_IpField.SetText(std::string(IpAddress::LocalAddress().ToPhrase().View()));
	m_IpField.SetPosition(pos);
}

void ConnectionState::InitUsernameField(const sf::Vector2f& pos)
{
	std::string nameLabel = "Username";
	m_IpField.SetSize(FIELD_SIZE);
	m_UsernameField.SetLabel(nameLabel);
	m_UsernameField.SetPosition(pos);
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
	unsigned int textSize = TEXT_SIZE;

	m_ShowLobbyBtn.SetSize(BUTTON_SIZE_EXTRA_EXTENDED_SMALL_Y);
	m_ShowLobbyBtn.SetPosition(pos);
	m_ShowLobbyBtn.SetColor(Emerald);
	m_ShowLobbyBtn.SetButtonText(btnText, TEXT_COLOR, textSize / 2.f);
}

void ConnectionState::InitQuickGameButton(const sf::Vector2f& pos)
{
	sf::Color Emerald(1, 215, 88);
	std::string btnText = "Quick Connect";
	unsigned int textSize = TEXT_SIZE;

	m_QuickGameBtn.SetSize(BUTTON_SIZE_EXTRA_EXTENDED_SMALL_Y);
	m_QuickGameBtn.SetPosition(pos);
	m_QuickGameBtn.SetColor(Emerald);
	m_QuickGameBtn.SetButtonText(btnText, TEXT_COLOR, textSize / 2.f);
}

void ConnectionState::ShowLobbyFunction()
{
	if (m_IsTryingToConnect) return;

	m_UsernameField.ClearErrorMessage();
	m_IpField.ClearErrorMessage();

	bool isNameValid = false;
	if (m_UsernameField.GetText().empty())
	{
		//DebugLog("Username should not be empty!\n");
		m_UsernameField.ShowErrorMessage("Username should not be empty!");
	}
	else if (m_UsernameField.GetText().size() < 3)
	{
		//DebugLog("Username should be more than 2 characters!\n");
		m_UsernameField.ShowErrorMessage("Username should be more than 2 characters!");
	}
	else
	{
		isNameValid = true;
	}

	m_Name = m_UsernameField.GetText();

	if (isNameValid)
	{
		// m_clientApp->ConnectToServer("127.0.0.1");
		m_ClientApp.ConnectToServer(m_IpField.GetText());
		m_IsTryingToConnect = true;
	}
	else
	{
		m_IpField.ShowErrorMessage("Invalid phrase!");
	}
}

#include "ConnectionState.h"
#include "MenuState.h"
#include "GameState.h"

constexpr float CONNECTION_TIMEOUT_TIME = 5.0f;

ConnectionState::ConnectionState(ClientApp& app)
	: m_ClientApp(app)
	, m_CurrentFunction(ButtonFunction::None)
	, m_BackBtn(app.GetFont(), app.GetInputHandler())
	, m_ConnectBtn(app.GetFont(), app.GetInputHandler())
	, m_IpField(app.GetFont(), app.GetInputHandler())
	, m_UsernameField(app.GetFont(), app.GetInputHandler())
{
	sf::Vector2f sizeField = FIELD_SIZE;
	float yOffset = app.GetWindow().GetHeight() * 0.1f;
	float xMiddleScreen = app.GetWindow().GetWidth() * 0.5f;

	sf::Vector2f fieldPos = sf::Vector2f(xMiddleScreen, 3.f * yOffset);

	// Init all graphics element
	sf::Vector2f backButtonPos = sf::Vector2f(xMiddleScreen, 6.f * yOffset);
	sf::Vector2f connectButtonPos = sf::Vector2f(xMiddleScreen, 8.f * yOffset);

	InitIpField(fieldPos);
	fieldPos.y += 1.5f * yOffset;

	InitUsernameField(fieldPos);

	InitBackButton(backButtonPos);
	InitConnectButton(connectButtonPos);
}

void ConnectionState::OnEnter(ClientApp& app)
{
	app.GetWindow().RegisterDrawable(m_IpField);
	app.GetWindow().RegisterDrawable(m_UsernameField);

	app.GetWindow().RegisterDrawable(m_BackBtn);
	app.GetWindow().RegisterDrawable(m_ConnectBtn);
}

void ConnectionState::OnUpdate(ClientApp& app, float dt)
{
	m_IpField.Update(dt, app.GetWindow());
	m_UsernameField.Update(dt, app.GetWindow());
	m_BackBtn.Update(dt, app.GetWindow());
	m_ConnectBtn.Update(dt, app.GetWindow());

	if (m_IsTryingToConnect)
	{
		ConnectionStateInfos connectionState = app.GetConnectionStateInfo();

		switch (connectionState)
		{
		case ConnectionStateInfos::FailedConnection:
		{
			m_IsTryingToConnect = false;
			break;
		}

		case ConnectionStateInfos::IsConnected:
		{
			m_CurrentFunction = ButtonFunction::GameScreen;
			break;
		}

		default:
			break;
		}
	}

	ActiveButtonFunction(app);
}

void ConnectionState::OnExit(ClientApp& app)
{
	app.GetWindow().UnregisterDrawable(m_IpField);
	app.GetWindow().UnregisterDrawable(m_UsernameField);
	app.GetWindow().UnregisterDrawable(m_BackBtn);
	app.GetWindow().UnregisterDrawable(m_ConnectBtn);
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
	std::function<void()> function = [this]()
	{
		m_CurrentFunction = ButtonFunction::MenuScreen;
	};

	m_BackBtn.SetSize(BUTTON_SIZE_EXTRA_EXTENDED);
	m_BackBtn.SetPosition(pos);
	m_BackBtn.SetColor(OrangeRed);
	m_BackBtn.SetButtonText(btnText, m_ClientApp.GetFont());
	m_BackBtn.SetOnClickCallback(function);
}

void ConnectionState::InitConnectButton(const sf::Vector2f& pos)
{
	sf::Color Emerald(1, 215, 88);
	std::string btnText = "Connect";
	std::function<void()> function = [this]()
	{
		m_CurrentFunction = ButtonFunction::Connect;
	};

	m_ConnectBtn.SetSize(BUTTON_SIZE_EXTENDED);
	m_ConnectBtn.SetPosition(pos);
	m_ConnectBtn.SetColor(Emerald);
	m_ConnectBtn.SetButtonText(btnText, m_ClientApp.GetFont());
	m_ConnectBtn.SetOnClickCallback(function);
}

void ConnectionState::ActiveButtonFunction(ClientApp& app)
{
	switch (m_CurrentFunction)
	{
	case ButtonFunction::MenuScreen:
	{
		m_ClientApp.ChangeState<MenuState>(app);
		break;
	}
	case ButtonFunction::Connect:
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
		break;
	}
	case ButtonFunction::GameScreen:
	{
		m_ClientApp.ChangeState<GameState>(app.GetFont());
		break;
	}
	}
}

#include "MenuState.h"
#include "ClientApp.h"
#include "ConnectionState.h"
//#include "LobbyState.h"

MenuState::MenuState(ClientApp& app)
	: m_ClientApp(app)
	, m_ConnectBtn(app.GetFont(), app.GetInputHandler())
	, m_DisconnectBtn(app.GetFont(), app.GetInputHandler())
	, m_QuitBtn(app.GetFont(), app.GetInputHandler())
	, m_Slider(app.GetInputHandler(), app.GetMusic())
	, m_IpField(app.GetFont(), app.GetInputHandler())
	, m_UsernameField(app.GetFont(), app.GetInputHandler())
	, m_SliderText(app.GetFont())
	, m_executeFunction()
	, m_callFunction(false)
{
	float yOffset = app.GetWindow().GetHeight() * 0.1f;
	float xMiddleScreen = app.GetWindow().GetWidth() * 0.5f;


	sf::Vector2f fieldPos = sf::Vector2f(xMiddleScreen, 3.f * yOffset);
	sf::Vector2f btnScreenPos = sf::Vector2f(xMiddleScreen, 6.f * yOffset);
	sf::Vector2f sliderpos = sf::Vector2f(app.GetWindow().GetWidth() * 0.8f, yOffset);

	InitSlider(sliderpos, app.GetWindow().GetWidth() * 0.1f, 0, 100, app.GetFont());

	InitIpField(fieldPos);
	fieldPos.y += 1.5f * yOffset;

	InitUsernameField(fieldPos);
	InitConnectionButton(btnScreenPos);

	btnScreenPos.y += yOffset;
	InitDisconnectButton(btnScreenPos);

	btnScreenPos.y += yOffset;
	InitQuitButton(btnScreenPos);
}

void MenuState::OnEnter(ClientApp& app)
{
	app.GetWindow().RegisterDrawable(m_IpField);
	app.GetWindow().RegisterDrawable(m_UsernameField);

	if (/*ClientConnectionHandler::GetInstance().IsConnected()*/ false)
	{
		app.GetWindow().RegisterDrawable(m_DisconnectBtn);
	}
	else
	{
		app.GetWindow().RegisterDrawable(m_ConnectBtn);
	}
	app.GetWindow().RegisterDrawable(m_QuitBtn);
	app.GetWindow().RegisterDrawable(m_Slider);
	app.GetWindow().RegisterDrawable(m_SliderText);
}

void MenuState::OnUpdate(ClientApp& app, float dt)
{
	m_IpField.Update(dt, app.GetWindow());
	m_UsernameField.Update(dt, app.GetWindow());
	m_ConnectBtn.Update(dt, app.GetWindow());
	m_DisconnectBtn.Update(dt, app.GetWindow());
	m_QuitBtn.Update(dt, app.GetWindow());
	m_Slider.Update(dt, app.GetWindow());

	if (m_ConnectBtn.IsPressed())
	{
		ShowLobbyFunction();
		return;
	}
	else if (m_DisconnectBtn.IsPressed())
	{
		m_ClientApp.GetWindow().UnregisterDrawable(m_ConnectBtn);
		m_DisconnectBtn.SetPosition(m_ConnectBtn.GetPosition());
		m_ClientApp.GetWindow().UnregisterDrawable(m_DisconnectBtn);
		return;
	}
	else if (m_QuitBtn.IsPressed())
	{
		m_Slider.SaveVolumeValue();
		m_ClientApp.Shutdown();
		return;
	}
}

void MenuState::OnExit(ClientApp& app)
{
	app.GetWindow().UnregisterDrawable(m_IpField);
	app.GetWindow().UnregisterDrawable(m_UsernameField);

	app.GetWindow().UnregisterDrawable(m_ConnectBtn);
	app.GetWindow().UnregisterDrawable(m_DisconnectBtn);
	app.GetWindow().UnregisterDrawable(m_QuitBtn);
	app.GetWindow().UnregisterDrawable(m_Slider);
	app.GetWindow().UnregisterDrawable(m_SliderText);

	m_Slider.SaveVolumeValue();
}

void MenuState::InitIpField(const sf::Vector2f& pos)
{
	std::string ipLabel = "Server Phrase";
	m_IpField.SetSize(FIELD_SIZE);
	m_IpField.SetLabel(ipLabel);
	m_IpField.SetText(std::string(IpAddress::LocalAddress().ToPhrase().View()));
	m_IpField.SetPosition(pos);
}

void MenuState::InitUsernameField(const sf::Vector2f& pos)
{
	std::string nameLabel = "Username";
	m_IpField.SetSize(FIELD_SIZE);
	m_UsernameField.SetLabel(nameLabel);
	m_UsernameField.SetPosition(pos);
}

void MenuState::InitSlider(sf::Vector2f& pos, float width, float minValue, float maxValue, const sf::Font& font)
{
	std::string musicText = "Music Volume";
	sf::Vector2f newPos = sf::Vector2f(pos.x + 80.f, pos.y - 40.f);

	m_Slider.SetPosition(pos);
	m_Slider.SetWidth(width);
	m_SliderText = TextComponent(font);
	m_SliderText.SetPosition(newPos);
	m_SliderText.SetText(musicText);
}

void MenuState::InitConnectionButton(const sf::Vector2f& pos)
{
	sf::Color Lime(24, 165, 88);
	std::string btnText = "Connection";

	m_ConnectBtn.SetSize(BUTTON_SIZE_EXTRA_EXTENDED);
	m_ConnectBtn.SetPosition(pos);
	m_ConnectBtn.SetColor(Lime);
	m_ConnectBtn.SetButtonText(btnText);
}

void MenuState::InitDisconnectButton(const sf::Vector2f& pos)
{
	std::string btnText = "Disconnect";

	m_DisconnectBtn.SetSize(BUTTON_SIZE_EXTRA_EXTENDED);
	m_DisconnectBtn.SetPosition(pos);
	m_DisconnectBtn.SetColor(sf::Color::Red);
	m_DisconnectBtn.SetButtonText(btnText);
}

void MenuState::InitQuitButton(const sf::Vector2f& pos)
{
	sf::Color OrangeRed(231, 62, 1);
	std::string btnText = "Quit";

	m_QuitBtn.SetSize(BUTTON_SIZE_EXTENDED);
	m_QuitBtn.SetPosition(pos);
	m_QuitBtn.SetColor(OrangeRed);
	m_QuitBtn.SetButtonText(btnText);
}

void MenuState::ShowLobbyFunction()
{
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
	}
	else
	{
		m_IpField.ShowErrorMessage("Invalid phrase!");
	}
}
#include "MenuState.h"
#include "ClientApp.h"
#include "ConnectionState.h"

MenuState::MenuState(ClientApp& app)
	: m_PlayBtn(app.GetFont(), app.GetInputHandler())
	, m_ConnectBtn(app.GetFont(), app.GetInputHandler())
	, m_DisconnectBtn(app.GetFont(), app.GetInputHandler())
	, m_QuitBtn(app.GetFont(), app.GetInputHandler())
	, m_Slider(app.GetInputHandler(), app.GetMusic())
	, m_SliderText(app.GetFont())
	, m_CurrentFunction(ButtonFunction::None)
{
	float yOffset = app.GetWindow().GetHeight() * 0.1f;
	float xMiddleScreen = app.GetWindow().GetWidth() * 0.5f;

	sf::Vector2f btnScreenPos = sf::Vector2f(xMiddleScreen, 4.f * yOffset);
	sf::Vector2f sliderpos = sf::Vector2f(app.GetWindow().GetWidth() * 0.8f, yOffset);

	InitSlider(sliderpos, app.GetWindow().GetWidth() * 0.1f, 0, 100, app.GetFont());
	InitConnectionButton(btnScreenPos, app.GetFont());
	InitPlayButton(btnScreenPos, app.GetFont());

	btnScreenPos.y += yOffset;
	InitDisconnectButton(btnScreenPos, app.GetFont());

	btnScreenPos.y += yOffset;
	InitQuitButton(btnScreenPos, app.GetFont());
}

void MenuState::OnEnter(ClientApp& app)
{
	if (/*ClientConnectionHandler::GetInstance().IsConnected()*/ false)
	{
		app.GetWindow().RegisterDrawable(m_PlayBtn);
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
	m_PlayBtn.Update(dt);
	m_ConnectBtn.Update(dt);
	m_DisconnectBtn.Update(dt);
	m_QuitBtn.Update(dt);
	m_Slider.Update(dt);

	ActiveButtonFunction(app);
}

void MenuState::OnExit(ClientApp& app)
{
	app.GetWindow().UnregisterDrawable(m_PlayBtn);
	app.GetWindow().UnregisterDrawable(m_ConnectBtn);
	app.GetWindow().UnregisterDrawable(m_DisconnectBtn);
	app.GetWindow().UnregisterDrawable(m_QuitBtn);
	app.GetWindow().UnregisterDrawable(m_Slider);
	app.GetWindow().UnregisterDrawable(m_SliderText);

	m_Slider.SaveVolumeValue();
}

void MenuState::InitSlider(sf::Vector2f& pos, float width, float minValue, float maxValue, const sf::Font& font)
{
	std::string musicText = "Music Volume";
	sf::Vector2f newPos = sf::Vector2f(pos.x + 10.f, pos.y - 40.f);

	m_Slider.SetPosition(pos);
	m_Slider.SetWidth(width);
	m_SliderText = TextComponent(font);
	m_SliderText.SetPosition(newPos);
	m_SliderText.SetText(musicText);
}

void MenuState::InitPlayButton(const sf::Vector2f& pos, const sf::Font& font)
{
	sf::Color Emerald(1, 215, 88);
	std::string btnText = "Play";
	std::function<void()> function = [this]()
	{
		m_CurrentFunction = ButtonFunction::Play;
	};

	m_PlayBtn.SetSize(BUTTON_SIZE_EXTENDED);
	m_PlayBtn.SetPosition(pos);
	m_PlayBtn.SetColor(Emerald);
	m_PlayBtn.SetButtonText(btnText, font);
	m_PlayBtn.SetOnClickCallback(function);
}

void MenuState::InitConnectionButton(const sf::Vector2f& pos, const sf::Font& font)
{
	sf::Color Lime(24, 165, 88);
	std::string btnText = "Connection";
	std::function<void()> function = [this]()
	{
		m_CurrentFunction = ButtonFunction::ConnectionScreen;
	};

	m_ConnectBtn.SetSize(BUTTON_SIZE_EXTRA_EXTENDED);
	m_ConnectBtn.SetPosition(pos);
	m_ConnectBtn.SetColor(Lime);
	m_ConnectBtn.SetButtonText(btnText, font);
	m_ConnectBtn.SetOnClickCallback(function);
}

void MenuState::InitDisconnectButton(const sf::Vector2f& pos, const sf::Font& font)
{
	std::string btnText = "Disconnect";
	std::function<void()> function = [this, pos]()
	{
		m_CurrentFunction = ButtonFunction::Disconnect;
	};

	m_DisconnectBtn.SetSize(BUTTON_SIZE_EXTRA_EXTENDED);
	m_DisconnectBtn.SetPosition(pos);
	m_DisconnectBtn.SetColor(sf::Color::Red);
	m_DisconnectBtn.SetButtonText(btnText, font);
	m_DisconnectBtn.SetOnClickCallback(function);
}

void MenuState::InitQuitButton(const sf::Vector2f& pos, const sf::Font& font)
{
	sf::Color OrangeRed(231, 62, 1);
	std::string btnText = "Quit";
	std::function<void()> function = [this]()
	{
		m_CurrentFunction = ButtonFunction::Quit;
	};

	m_QuitBtn.SetSize(BUTTON_SIZE_EXTENDED);
	m_QuitBtn.SetPosition(pos);
	m_QuitBtn.SetColor(OrangeRed);
	m_QuitBtn.SetButtonText(btnText, font);
	m_QuitBtn.SetOnClickCallback(function);
}

void MenuState::ActiveButtonFunction(ClientApp& app)
{
	switch (m_CurrentFunction)
	{
	case ButtonFunction::Play:
	{
		//ChangedState<LobbyState>("LobbyState");
		break;
	}
	case ButtonFunction::ConnectionScreen:
	{
		app.ChangeState<ConnectionState>(app);
		break;
	}
	case ButtonFunction::Disconnect:
	{
		app.GetWindow().UnregisterDrawable(m_ConnectBtn);
		m_DisconnectBtn.SetPosition(m_ConnectBtn.GetPosition());
		app.GetWindow().UnregisterDrawable(m_DisconnectBtn);
		break;
	}
	case ButtonFunction::Quit:
	{
		m_Slider.SaveVolumeValue();
		app.Shutdown();
		break;
	}
	default:
	{
		break;
	}
	}
}
#include "MenuState.h"

#include "ClientApp.h"
#include "../../FontRegistry.h"
#include "ConnectionState.h"

#pragma region  Constructor

MenuState::MenuState(ClientApp& app)
    : m_clientApp(&app)
    , _sliderText(*app.GetFontByName(FONT_DEFAULT))
    , _currentFunction(ButtonFunction::None)
{
    float yOffset = app.GetWindow()->GetHeight() * 0.1f;
    float xMiddleScreen = app.GetWindow()->GetWidth() * 0.5f;

    sf::Vector2f btnScreenPos = sf::Vector2f(xMiddleScreen, 4.f * yOffset);
    sf::Vector2f sliderpos = sf::Vector2f(app.GetWindow()->GetWidth() * 0.8f, yOffset);
    
    InitSlider(sliderpos, app.GetWindow()->GetWidth() * 0.1f, 0, 100);
    InitConnectionButton(btnScreenPos);
    InitPlayButton(btnScreenPos);

    btnScreenPos.y += yOffset;
    InitDisconnectButton(btnScreenPos);

    btnScreenPos.y += yOffset;
    InitQuitButton(btnScreenPos);
}

MenuState::~MenuState()
{

}

#pragma endregion

#pragma region  Override

void MenuState::OnEnter(ClientApp& app)
{
    if (/*ClientConnectionHandler::GetInstance().IsConnected()*/ false)
    {
        m_clientApp->GetWindow()->RegisterDrawable(PlayBtn);
        m_clientApp->GetWindow()->RegisterDrawable(DisconnectBtn);
    }
    else
    {
        m_clientApp->GetWindow()->RegisterDrawable(ConnectBtn);
    }
    m_clientApp->GetWindow()->RegisterDrawable(QuitBtn);
    m_clientApp->GetWindow()->RegisterDrawable(_slider);
    m_clientApp->GetWindow()->RegisterDrawable(_sliderText);
}

void MenuState::OnUpdate(ClientApp& app, float dt)
{
    for (auto& btn : _btns)
    {
        btn.Update(dt);
    }

    _slider.Update(dt);

    ActiveButtonFunction(app);
}

void MenuState::OnExit(ClientApp& app)
{
    for (auto& btn : _btns)
    {
        app.GetWindow()->UnregisterDrawable(btn);
    }
    app.GetWindow()->UnregisterDrawable(_slider);
    app.GetWindow()->UnregisterDrawable(_sliderText);

    _slider.SaveVolumeValue();

    m_clientApp = nullptr;
}

#pragma endregion

#pragma region  Class Methods

void MenuState::InitSlider(sf::Vector2f& pos, float width, float minValue, float maxValue)
{
    std::string musicText = "Music Volume";
    sf::Vector2f newPos = sf::Vector2f(pos.x + 10.f, pos.y - 40.f);

    _slider = SliderComponent(pos, width, minValue, maxValue, m_clientApp->GetInputHandler(), m_clientApp->GetMusic());
    _sliderText = TextComponent(*m_clientApp->GetFontByName(FONT_DEFAULT));
    _sliderText.SetPosition(newPos);
    _sliderText.SetText(musicText);
}

void MenuState::InitPlayButton(const sf::Vector2f& pos)
{
    sf::Color Emerald(1, 215, 88);
    std::string btnText = "Play";
    std::function<void()> function = [this]()
        {
            _currentFunction = ButtonFunction::Play;
        };

    const sf::Font& font = *m_clientApp->GetFontByName(FONT_DEFAULT);

    new (&PlayBtn) ButtonComponent(pos, font, Emerald, m_clientApp->GetInputHandler());
    PlayBtn.SetButtonText(btnText, font);
    PlayBtn.SetOnClickCallback(function);
}

void MenuState::InitConnectionButton(const sf::Vector2f& pos)
{
    sf::Color Emerald(1, 215, 88);
    //sf::Color Lime(24, 165, 88);
    sf::Vector2f size(300,100);
    std::string btnText = "Connection";
    std::function<void()> function = [this]()
        {
            _currentFunction = ButtonFunction::ConnectionScreen;
        };

    const sf::Font& font = *m_clientApp->GetFontByName(FONT_DEFAULT);

    new (&ConnectBtn) ButtonComponent(pos, font, Emerald, m_clientApp->GetInputHandler());
    ConnectBtn.SetButtonText(btnText, font);
    ConnectBtn.SetOnClickCallback(function);
}

void MenuState::InitDisconnectButton(const sf::Vector2f& pos)
{
    std::string btnText = "Disconnect";
    std::function<void()> function = [this, pos]()
        {
            _currentFunction = ButtonFunction::Disconnect;
        };

    const sf::Font& font = *m_clientApp->GetFontByName(FONT_DEFAULT);

    new (&DisconnectBtn) ButtonComponent(pos, font, sf::Color::Red, m_clientApp->GetInputHandler());
    DisconnectBtn.SetButtonText(btnText, font);
    DisconnectBtn.SetOnClickCallback(function);
}

void MenuState::InitQuitButton(const sf::Vector2f& pos)
{
    sf::Color OrangeRed(231, 62, 1);
    std::string btnText = "Quit";
    std::function<void()> function = [this]()
        {
            _currentFunction = ButtonFunction::Quit;
        };

    const sf::Font& font = *m_clientApp->GetFontByName(FONT_DEFAULT);

    new (&QuitBtn) ButtonComponent(pos, font, OrangeRed, m_clientApp->GetInputHandler());
    QuitBtn.SetButtonText(btnText, font);
    QuitBtn.SetOnClickCallback(function);
    m_clientApp->GetWindow()->RegisterDrawable(QuitBtn);
}

void MenuState::ActiveButtonFunction(ClientApp& app)
{
    switch (_currentFunction)
    {
        case ButtonFunction::Play: 
        {
            //ChangedState<LobbyState>("LobbyState");
            break;
        }
        case ButtonFunction::ConnectionScreen: 
        {
            m_clientApp->ChangeState<ConnectionState>(app);
            break;
        }
        case ButtonFunction::Disconnect: 
        {
            m_clientApp->GetWindow()->UnregisterDrawable(ConnectBtn);
            DisconnectBtn.SetPosition(ConnectBtn.GetPosition());
            m_clientApp->GetWindow()->UnregisterDrawable(DisconnectBtn);
            break;
        }
        case ButtonFunction::Quit: 
        {
            _slider.SaveVolumeValue();
            m_clientApp->Shutdown();
            break;
        }     
        default: 
        {
            break;
        }   
    }
}

#pragma endregion
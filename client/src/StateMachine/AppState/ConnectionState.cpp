#include "ConnectionState.h"
#include "../../FontRegistry.h"
#include "MenuState.h"
#include "GameState.h"

constexpr float CONNECTION_TIMEOUT_TIME = 5.0f;

#pragma region  Constructor

ConnectionState::ConnectionState(ClientApp& app)
    : m_clientApp(&app)
    , m_font(app.GetFontByName(FONT_DEFAULT))
    , _currentFunction(ButtonFunction::None)
{
    sf::Vector2f sizeField = FIELD_SIZE;
    float yOffset = app.GetWindow()->GetHeight() * 0.1f;
    float xMiddleScreen = app.GetWindow()->GetWidth() * 0.5f;

    sf::Vector2f fieldPos = sf::Vector2f(xMiddleScreen, 3.f * yOffset);

    // Init all graphics element
    sf::Vector2f backButtonPos = sf::Vector2f(xMiddleScreen, 6.f * yOffset);
    sf::Vector2f connectButtonPos = sf::Vector2f(xMiddleScreen, 8.f * yOffset);

    InitIpField(fieldPos);
    fieldPos.y += yOffset;

    InitUsernameField(fieldPos);

    InitBackButton(backButtonPos);
    InitConnectButton(connectButtonPos);
}

ConnectionState::~ConnectionState()
{

}

#pragma endregion

#pragma region  Override

void ConnectionState::OnEnter(ClientApp& app)
{
    m_clientApp->GetWindow()->RegisterDrawable(IpField);
    m_clientApp->GetWindow()->RegisterDrawable(UsernameField);

    m_clientApp->GetWindow()->RegisterDrawable(BackBtn);
    m_clientApp->GetWindow()->RegisterDrawable(ConnectBtn);
}

void ConnectionState::OnUpdate(ClientApp& app, float dt)
{
    for (auto& field : _fields)
    {
        field.Update(dt);
    }

    for (auto& btn : _btns)
    {
        btn.Update(dt);
    }

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
                _currentFunction = ButtonFunction::GameScreen;
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
    for (auto& btn : _btns)
    {
        app.GetWindow()->UnregisterDrawable(btn);
    }

    for (auto& field : _fields)
    {
        app.GetWindow()->UnregisterDrawable(field);
    }
}


#pragma endregion

#pragma region  Class Methods

void ConnectionState::InitIpField(const sf::Vector2f& pos)
{
    std::string ipLabel = "Server Phrase";

    new (&IpField) InsertFieldComponent(*m_font, m_clientApp->GetInputHandler());
    IpField.SetPosition(pos);
    IpField.SetLabel(ipLabel);
}

void ConnectionState::InitUsernameField(const sf::Vector2f& pos)
{
    std::string nameLabel = "Username";

    new (&UsernameField) InsertFieldComponent(*m_font, m_clientApp->GetInputHandler());
    UsernameField.SetPosition(pos);
    UsernameField.SetLabel(nameLabel);
}

void ConnectionState::InitBackButton(const sf::Vector2f& pos)
{
    sf::Color OrangeRed(231, 62, 1);
    std::string btnText = "Return to menu";
    std::function<void()> function = [this]()
        {
            _currentFunction = ButtonFunction::MenuScreen;
        };

    new (&BackBtn) ButtonComponent(pos, *m_font, OrangeRed, m_clientApp->GetInputHandler(), BUTTON_SIZE_EXTRA_EXTENDED);
    BackBtn.SetButtonText(btnText, *m_font);
    BackBtn.SetOnClickCallback(function);
}

void ConnectionState::InitConnectButton(const sf::Vector2f& pos)
{
    sf::Color Emerald(1, 215, 88);
    std::string btnText = "Connect";
    std::function<void()> function = [this]()
        {
            _currentFunction = ButtonFunction::Connect;            
        };

    new (&ConnectBtn) ButtonComponent(pos, *m_font, Emerald, m_clientApp->GetInputHandler());
    ConnectBtn.SetButtonText(btnText, *m_font);
    ConnectBtn.SetOnClickCallback(function);
}

void ConnectionState::ActiveButtonFunction(ClientApp& app)
{
    switch (_currentFunction)
    {
        case ButtonFunction::MenuScreen:
        {
            m_clientApp->ChangeState<MenuState>(app);
            break;
        }
        case ButtonFunction::Connect:
        {
            if (m_IsTryingToConnect) return;

            for (auto& field : _fields)
            {
                field.ClearErrorMessage();
            }

            bool isNameValid = false;
            if (UsernameField.GetText().empty())
            {
                //DebugLog("Username should not be empty!\n");
                UsernameField.ShowErrorMessage("Username should not be empty!");
            }
            else if (UsernameField.GetText().size() < 3)
            {
                //DebugLog("Username should be more than 2 characters!\n");
                UsernameField.ShowErrorMessage("Username should be more than 2 characters!");
            }
            else
            {
                isNameValid = true;
            }

            m_Name = UsernameField.GetText();

            if (isNameValid)
            {
                // m_clientApp->ConnectToServer("127.0.0.1");
                m_clientApp->ConnectToServer(IpField.GetText());
                m_IsTryingToConnect = true;
            }
            else
            {
                IpField.ShowErrorMessage("Invalid phrase!");
            }
            break;
        }
        case ButtonFunction::GameScreen:
        {
            m_clientApp->ChangeState<GameState>(*app.GetFontByName("JuliaMono-Regular.ttf"));
            break;
        }
        default:
        {
            break;
        }
    }
}

#pragma endregion

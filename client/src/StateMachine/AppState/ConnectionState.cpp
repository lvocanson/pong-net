#include "ConnectionState.h"

#include <regex>
#include "../../FontRegistry.h"
#include "MenuState.h"
#include "GameState.h"

constexpr float CONNECTION_TIMEOUT_TIME = 5.0f;

#pragma region  Constructor

ConnectionState::ConnectionState()
    : m_clientApp(nullptr)
    , m_font(nullptr)
{
    _btns = std::vector<ButtonComponent*>();
    _fields = std::vector<InsertFieldComponent*>();
    _currentFunction = ButtonFunction::None;
}

ConnectionState::~ConnectionState()
{

}

#pragma endregion

#pragma region  Override

void ConnectionState::OnEnter(ClientApp& app)
{
    m_clientApp = &app;
    m_font = m_clientApp->GetFontByName(FONT_DEFAULT);
    Window *m_window = m_clientApp->GetWindow();

    sf::Vector2f sizeField = FIELD_SIZE;

    float xBtnExtended = m_window->GetWidth() * 0.5f - (BUTTON_SIZE_EXTENDED.x * 0.5f);
    float xBtnExtraExtended = m_window->GetWidth() * 0.5f - (BUTTON_SIZE_EXTRA_EXTENDED.x * 0.5f);

    float xField = m_window->GetWidth() * 0.5f - (sizeField.x * 0.5f);
    float yOffset = 100.f;

    sf::Vector2f fieldPos = sf::Vector2f(xField, 2.f * yOffset);

    // Init all graphics element
    sf::Vector2f backButtonPos = sf::Vector2f(xBtnExtraExtended, 4.f * yOffset);
    sf::Vector2f connectButtonPos = sf::Vector2f(xBtnExtended, 6.f * yOffset);

    ShowIpField(fieldPos);
    fieldPos.y += yOffset;

    ShowNameField(fieldPos);

    ShowBackButton(backButtonPos);

    ShowConnectButton(connectButtonPos);
}

void ConnectionState::OnUpdate(ClientApp& app, float dt)
{
    for (auto field : _fields)
    {
        field->Update(dt);
    }

    for (auto btn : _btns)
    {
        btn->Update(dt);
    }

    if (m_IsTryingToConnect)
    {
        _currentFunction = ButtonFunction::GameScreen;
    }

    ActiveButtonFunction();
}

void ConnectionState::OnExit(ClientApp& app)
{
    for (auto it = _btns.begin(); it != _btns.end(); ++it)
    {
        app.GetWindow()->UnregisterDrawable(*it);
        delete *it;
    }
    _btns.clear();

    for (auto it = _fields.begin(); it != _fields.end(); ++it)
    {
        app.GetWindow()->UnregisterDrawable(*it);
        delete* it;
    }
    _fields.clear();
}


#pragma endregion

#pragma region  Class Methods

void ConnectionState::AddButton(const sf::Vector2f& pos, const sf::Color& color, const std::string& text, sf::Font* font, std::function<void()> function, const sf::Vector2f& size)
{
    ButtonComponent* btn = new ButtonComponent(pos, color, m_clientApp->GetInputHandler(),size);
    btn->SetButtonText(text, *font);
    btn->SetOnClickCallback(function);

    m_clientApp->GetWindow()->RegisterDrawable(btn);
    _btns.push_back(btn);
}

void ConnectionState::AddField(const sf::Vector2f& pos, const std::string& label, sf::Font* font)
{
    InsertFieldComponent* field = new InsertFieldComponent(*font, m_clientApp->GetInputHandler());
    field->SetPosition(pos);
    field->SetLabel(label);

    m_clientApp->GetWindow()->RegisterDrawable(field);
    _fields.push_back(field);
}

InsertFieldComponent* ConnectionState::FindFieldByText(const std::string& text)
{
    for (auto field : _fields)
    {
        sf::Text textLabel = field->GetLabelComponent()->GetText();
        if (textLabel.getString() == text)
        {
            return field;
        }
    }
    return nullptr;
}

void ConnectionState::ShowIpField(const sf::Vector2f& pos)
{
    std::string ipLabel = "Server Phrase";
    AddField(pos, ipLabel, m_font);

    //#if defined(DEBUG) | defined(_DEBUG)
    //_fields[_fields.size() - 1]->SetText(TcpIp::IpAddress::FromString(sf::IpAddress::getLocalAddress().toString()).ToPhrase());
    //#endif
}

void ConnectionState::ShowNameField(const sf::Vector2f& pos)
{
    std::string nameLabel = "Username";
    AddField(pos, nameLabel, m_font);
}

void ConnectionState::ShowBackButton(const sf::Vector2f& pos)
{
    sf::Color OrangeRed(231, 62, 1);
    std::string btnText = "Return to menu";
    sf::Vector2f size = BUTTON_SIZE_EXTRA_EXTENDED;

    std::function<void()> function = [this]()
        {
            _currentFunction = ButtonFunction::MenuScreen;
        };

    AddButton(pos, OrangeRed, btnText, m_font, function, size);
}

void ConnectionState::ShowConnectButton(const sf::Vector2f& pos)
{
    sf::Color Emerald(1, 215, 88);
    std::string btnText = "Connect";
    sf::Vector2f size = BUTTON_SIZE_EXTENDED;

    std::function<void()> function = [this]()
        {
            _currentFunction = ButtonFunction::Connect;            
        };

    AddButton(pos, Emerald, btnText, m_font, function, size);
}

void ConnectionState::ActiveButtonFunction()
{
    switch (_currentFunction)
    {
        case ButtonFunction::MenuScreen:
        {
            m_clientApp->ChangeState<MenuState>();
            break;
        }
        case ButtonFunction::Connect:
        {
            if (m_IsTryingToConnect) return;

            for (auto field : _fields)
            {
                field->ClearErrorMessage();
            }

            InsertFieldComponent* nameField = FindFieldByText("Username");

            bool isNameValid = false;
            if (nameField->GetText().empty())
            {
                //DebugLog("Username should not be empty!\n");
                nameField->ShowErrorMessage("Username should not be empty!");
            }
            else if (nameField->GetText().size() < 3)
            {
                //DebugLog("Username should be more than 2 characters!\n");
                nameField->ShowErrorMessage("Username should be more than 2 characters!");
            }
            else
            {
                isNameValid = true;
            }

            m_Name = nameField->GetText();

            InsertFieldComponent* ipField = FindFieldByText("Server Phrase");
            const std::string ip = ipField->GetText();

            if (isNameValid)
            {
                //m_clientApp->ConnectToServer(ip);
                m_clientApp->ConnectToServer("127.0.0.1");
                m_IsTryingToConnect = true;
            }
            else
            {
                ipField->ShowErrorMessage("Invalid phrase!");
            }
            break;
        }
        case ButtonFunction::GameScreen:
        {
            m_clientApp->ChangeState<GameState>();
            break;
        }
        default:
        {
            break;
        }
    }
}

bool ConnectionState::IsValidIpAddress(const char* ip)
{
    const std::regex ipRegex(R"(^\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}$)");
    return std::regex_match(ip, ipRegex) || std::string("LOCALHOST")._Equal(ip);
}

#pragma endregion

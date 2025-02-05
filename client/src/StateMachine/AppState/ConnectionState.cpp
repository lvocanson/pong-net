#include "ConnectionState.h"

#include <regex>
#include "../../../FontRegistry.h"
#include "../../../../common/CoreDefinitions.h"
#include "MenuState.h"

constexpr float CONNECTION_TIMEOUT_TIME = 5.0f;

#pragma region  Constructor

ConnectionState::ConnectionState()
{
    m_font = FontRegistry::GetFont();
    _btns = std::vector<ButtonComponent*>();
    _fields = std::vector<InsertFieldComponent*>();
}

ConnectionState::~ConnectionState()
{

}

#pragma endregion

#pragma region  Override

void ConnectionState::OnEnter(ClientApp& app)
{
    m_clientApp = &app;

    sf::Vector2f sizeBtn = BUTTON_SIZE;
    sf::Vector2f sizeField = FIELD_SIZE;

    float xBtn = app.GetWindow()->GetWidth() * 0.5f - (sizeBtn.x * 0.5f);
    float xField = app.GetWindow()->GetWidth() * 0.5f - (sizeField.x * 0.5f);
    float yOffset = 100.f;

    // Init all graphics element
    sf::Vector2f pos = sf::Vector2f(xField, 2.f * yOffset);

    ShowIpField(pos);
    pos.y += yOffset;

    ShowNameField(pos);
    pos.y += 2.f * yOffset;
    pos.x = xBtn;

    ShowConnectButton(pos);
    pos.y += 2.f * yOffset;

    ShowBackButton(pos);
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
        static float timeOutTimer;

        /* Shared<ConnectionStateInfo>& connectionInfo = ClientConnectionHandler::GetInstance().GetConnectionInfo();

         switch (connectionInfo.WaitGet().Get())
         {
         case Connecting:
         {
             if (timeOutTimer > CONNECTION_TIMEOUT_TIME)
             {
                 m_IpField->ShowErrorMessage("Connection timed out!");
                 m_IsTryingToConnect = false;
                 timeOutTimer = 0.0f;
             }
             else
             {
                 timeOutTimer += dt;
             }
             break;
         }
         case Failed:
         {
             m_IpField->ShowErrorMessage("Connection failed!");
             m_IsTryingToConnect = false;
             timeOutTimer = 0.0f;
             break;
         }
         case Connected:
         {
             ClientApp::GetInstance().GetCurrentPlayer()->SetName(m_NameField->GetText());

             m_StateMachine->SwitchState("LobbyState");
             m_IpField->ClearErrorMessage();
             m_IsTryingToConnect = false;

             Message<MsgType::Login> message;
             message.Username = m_Name;
             ClientConnectionHandler::GetInstance().SendDataToServer(message);

             timeOutTimer = 0.0f;
             break;
         }
         default:
             break;
         }*/
    }
}

void ConnectionState::OnExit(ClientApp& app)
{
    for (auto btn = _btns.begin(); btn != _btns.end();)
    {
        app.GetWindow()->UnregisterDrawable(*btn);
        RELEASE(*btn);  // Deletes the pointer and sets it to nullptr
        btn = _btns.erase(btn);  // erase() returns the next valid iterator
    }

    for (auto field = _fields.begin(); field != _fields.end();)
    {
        app.GetWindow()->UnregisterDrawable(*field);
        RELEASE(*field);  // Deletes the pointer and sets it to nullptr
        field = _fields.erase(field);  // erase() returns the next valid iterator
    }
}


#pragma endregion

#pragma region  Class Methods

void ConnectionState::AddButton(const sf::Vector2f& pos, const sf::Color& color, const std::string& text, sf::Font* font, std::function<void()> function)
{
    ButtonComponent* btn = new ButtonComponent(pos, color, m_clientApp->GetInputHandler());
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

    std::function<void()> function = [this]()
        {
            //ChangeState<MenuState>("MenuState");
        };

    AddButton(pos, OrangeRed, btnText, FontRegistry::GetFont(), function);
}

void ConnectionState::ShowConnectButton(const sf::Vector2f& pos)
{
    sf::Color Emerald(1, 215, 88);
    std::string btnText = "Connect";

    std::function<void()> function = [this]()
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

            //const std::string ip = std::string(TcpIp::IpAddress::FromPhrase(m_IpField->GetText()).ToString());
            //DebugLog(ip);

            if (isNameValid)
            {
                //ClientConnectionHandler::GetInstance().TryToConnectToServer(&ip);
                m_IsTryingToConnect = true;
            }
            else
            {
                InsertFieldComponent* ipField = FindFieldByText("Server Phrase");
                //DebugLog("Invalid phrase!\n");
                ipField->ShowErrorMessage("Invalid phrase!");
            }
        };

    AddButton(pos, Emerald, btnText, FontRegistry::GetFont(), function);
}

bool ConnectionState::IsValidIpAddress(const char* ip)
{
    const std::regex ipRegex(R"(^\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}$)");
    return std::regex_match(ip, ipRegex) || std::string("LOCALHOST")._Equal(ip);
}

#pragma endregion

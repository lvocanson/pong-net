#include "MenuState.h"

#include "ClientApp.h"
#include "../../../FontRegistry.h"
#include "../../../../common/CoreDefinitions.h"
#include "ConnectionState.h"

#pragma region  Constructor

MenuState::MenuState()
{
    _btns = std::vector<ButtonComponent*>();
}

MenuState::~MenuState()
{

}

#pragma endregion

#pragma region  Override

void MenuState::OnEnter(ClientApp& app)
{
    m_clientApp = &app;

    float yOffset = 100.f;
    sf::Vector2f size = BUTTON_SIZE;
    sf::Vector2f pos = sf::Vector2f(app.GetWindow()->GetWidth() * 0.5f - (size.x * 0.5f), 2.f * yOffset);

    if (/*ClientConnectionHandler::GetInstance().IsConnected()*/ false)
    {
        ShowPlayButton(pos);
        pos.y += yOffset;
        ShowDisconnectButton(pos);
    }
    else
    {
        ShowConnectionButton(pos);
    }
    pos.y += 2.f * yOffset;
    ShowQuitButton(pos);
}

void MenuState::OnUpdate(ClientApp& app, float dt)
{
    for (auto btn : _btns)
    {
        btn->Update(dt);
    }
}

void MenuState::OnExit(ClientApp& app)
{
    for (auto btn = _btns.begin(); btn != _btns.end();)
    {
        app.GetWindow()->UnregisterDrawable(*btn);
        RELEASE(*btn);  // Deletes the pointer and sets it to nullptr
        btn = _btns.erase(btn);  // erase() returns the next valid iterator
    }
}

#pragma endregion

#pragma region  Class Methods

void MenuState::AddButton(const sf::Vector2f& pos, const sf::Color& color, const std::string& text, sf::Font* font, std::function<void()> function)
{
    ButtonComponent* btn = new ButtonComponent(pos, color, m_clientApp->GetInputHandler());
    btn->SetButtonText(text, *FontRegistry::GetFont());
    btn->SetOnClickCallback(function);
    m_clientApp->GetWindow()->RegisterDrawable(btn);
    _btns.push_back(btn);
}

ButtonComponent* MenuState::FindButtonByText(const std::string& text)
{
    for (auto btn : _btns)
    {
        sf::Text textBtn = btn->GetTextComponent()->GetText();
        if (textBtn.getString() == text)
        {
            return btn;
        }
    }
    return nullptr;
}

void MenuState::ShowPlayButton(const sf::Vector2f& pos)
{
    sf::Color Emerald(1, 215, 88);
    std::string btnText = "Play";
    std::function<void()> function = [this]()
        {
            //ChangedState<LobbyState>("LobbyState");
        };

    AddButton(pos, Emerald, btnText, FontRegistry::GetFont(), function);
}

void MenuState::ShowConnectionButton(const sf::Vector2f& pos)
{
    sf::Color Lime(24, 165, 88);
    std::string btnText = "Connection";
    std::function<void()> function = [this]()
        {
            //ChangeState<ConnectionState>();
        };

    AddButton(pos, Lime, btnText, FontRegistry::GetFont(), function);
}

void MenuState::ShowDisconnectButton(const sf::Vector2f& pos)
{
    std::string btnText = "Disconnect";
    std::function<void()> function = [this, pos]()
        {
            //ClientConnectionHandler::GetInstance().Disconnect();
            ButtonComponent* btnConnect = FindButtonByText("Connection");
            m_clientApp->GetWindow()->UnregisterDrawable(btnConnect);
            RELEASE(btnConnect);

            ShowConnectionButton(pos);
            ButtonComponent* btnDisconnect = FindButtonByText("Disconnect");
            m_clientApp->GetWindow()->UnregisterDrawable(btnDisconnect);
        };

    AddButton(pos, sf::Color::Red, btnText, FontRegistry::GetFont(), function);
}

void MenuState::ShowQuitButton(const sf::Vector2f& pos)
{
    sf::Color OrangeRed(231, 62, 1);
    std::string btnText = "Quit";
    std::function<void()> function = [this]()
        {
            /* ClientApp::GetInstance().Shutdown();*/
        };

    AddButton(pos, OrangeRed, btnText, FontRegistry::GetFont(), function);
}

#pragma endregion
#include "LobbyState.h"
#include "ConnectionState.h"

LobbyState::LobbyState(ClientApp& app)
    : m_ClientApp(app)
    , m_ReturnButton(app.GetFont(), app.GetInputHandler())
    , m_LobbyButtons{}
{
    InitReturnButton(sf::Vector2f(500, 600));
}

void LobbyState::OnEnter(ClientApp& app)
{
    m_IsTryingToJoinLobby = false;
    m_IsLobbyInit = false;
    app.GetWindow().RegisterDrawable(m_ReturnButton);
}

void LobbyState::OnUpdate(ClientApp& app, float deltaTime)
{
    m_ReturnButton.Update(deltaTime, app.GetWindow());

    if (m_ReturnButton.IsPressed()) 
    {
        m_ClientApp.ChangeState<ConnectionState>(m_ClientApp);
    }
}

void LobbyState::OnExit(ClientApp& app)
{
}

void LobbyState::InitReturnButton(const sf::Vector2f& pos)
{
    sf::Color OrangeRed(231, 62, 1);
    std::string btnText = "Return to menu";

    m_ReturnButton.SetSize(BUTTON_SIZE_EXTRA_EXTENDED);
    m_ReturnButton.SetPosition(pos);
    m_ReturnButton.SetColor(OrangeRed);
    m_ReturnButton.SetButtonText(btnText, m_ClientApp.GetFont());
}

void LobbyState::JoinLobbyRequest(int lobbyID)
{
}

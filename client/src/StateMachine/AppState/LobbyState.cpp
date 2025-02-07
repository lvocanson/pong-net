#include "LobbyState.h"
#include "ConnectionState.h"
#include "GameState.h"
#include <Network/PacketWrapper.h>

LobbyState::LobbyState(ClientApp& app)
    : m_ClientApp(app)
    , m_ReturnButton(app.GetFont(), app.GetInputHandler())
{
    float width = app.GetWindow().GetWidth();
    float height = app.GetWindow().GetHeight();

    InitReturnButton(sf::Vector2f(width * 0.5f, height * 0.8f));

    m_LobbyBtns = std::vector<ButtonComponent>();

    sf::Vector2f newPos = sf::Vector2f(width * 0.5f, height * 0.5f);
    AddLobbyButton(newPos);
}

void LobbyState::OnEnter(ClientApp& app)
{
    m_IsTryingToJoinLobby = false;
    m_IsLobbyInit = false;

    app.GetWindow().RegisterDrawable(m_ReturnButton);

    for (auto& lobbyBtn : m_LobbyBtns)
    {
        app.GetWindow().RegisterDrawable(lobbyBtn);
    }
}

void LobbyState::OnUpdate(ClientApp& app, float deltaTime)
{
    m_ReturnButton.Update(deltaTime, app.GetWindow());

    for (auto& lobbyBtn : m_LobbyBtns) 
    {
        lobbyBtn.Update(deltaTime, app.GetWindow());
    }

    // Check button pressed

    if (m_ReturnButton.IsPressed()) 
    {
        m_ClientApp.ChangeState<ConnectionState>(m_ClientApp);
        return;
    }

    for (int i = 0; i < m_LobbyBtns.size(); i++)
    {
        if (m_LobbyBtns[i].IsPressed())
        {
            int indexGame = i;
            {
                Message request(MessageType::QuickMatchRequest);
                auto wrapper = PacketWrapper::Wrap(request);
                wrapper.Sign(m_ClientApp.GetSignature());
                wrapper.Send(m_ClientApp.GetSocket(), m_ClientApp.GetServerAddr());
            }
            //m_ClientApp.ChangeState<GameState>(m_ClientApp.GetFont());
            return;
        }
    }
}

void LobbyState::OnExit(ClientApp& app)
{
    app.GetWindow().UnregisterDrawable(m_ReturnButton);

    for (auto& lobbyBtn : m_LobbyBtns)
    {
        app.GetWindow().UnregisterDrawable(lobbyBtn);
    }

    m_LobbyBtns.clear();
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

void LobbyState::AddLobbyButton(const sf::Vector2f& pos)
{
    ButtonComponent& btn = m_LobbyBtns.emplace_back(m_ClientApp.GetFont(), m_ClientApp.GetInputHandler());
    sf::Color OrangeRed(128, 192, 128);
    if (m_LobbyBtns.size() % 2 == 1) 
    {
        OrangeRed += sf::Color(0, 64, 0);
    }
    std::string btnText = "Connect to lobby : " + std::to_string(m_LobbyBtns.size());

    btn.SetSize(BUTTON_LOBBY_SIZE);
    btn.SetPosition(pos);
    btn.SetColor(OrangeRed);
    btn.SetButtonText(btnText, m_ClientApp.GetFont());
}

void LobbyState::JoinLobbyRequest(int lobbyID)
{
    if (m_IsTryingToJoinLobby) return;

    m_IsTryingToJoinLobby = true;
}

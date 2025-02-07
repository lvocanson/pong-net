#include "LobbyState.h"
#include "ConnectionState.h"
#include "GameState.h"
#include <Network/PacketWrapper.h>

LobbyState::LobbyState(ClientApp& app)
	: m_ClientApp(app)
	, m_ReturnButton(app.GetFont(), app.GetInputHandler())
	, m_CreateButton(app.GetFont(), app.GetInputHandler())
	, m_LobbyBtns()
{
	float width = app.GetWindow().GetWidth();
	float height = app.GetWindow().GetHeight();

	InitCreateButton(sf::Vector2f(width * 0.75f, height * 0.8f));
	InitReturnButton(sf::Vector2f(width * 0.25f, height * 0.8f));
}

void LobbyState::OnEnter(ClientApp& app)
{
	m_IsTryingToJoinLobby = false;
	m_IsLobbyInit = false;

	app.GetWindow().RegisterDrawable(m_ReturnButton);
	app.GetWindow().RegisterDrawable(m_CreateButton);

	app.ResetRoomIds();

	Message_RoomGroupRequest request(0);
	auto wrapper = PacketWrapper::Wrap(request);
	wrapper.Sign(app.GetSignature());
	wrapper.Send(app.GetSocket(), app.GetServerAddr());
}

void LobbyState::OnUpdate(ClientApp& app, float deltaTime)
{
	const std::vector<uint16_t>& roomIds = app.GetRoomIds();
	sf::Vector2f firstButtonPos = sf::Vector2f(app.GetWindow().GetWidth() * 0.5f, app.GetWindow().GetHeight() * 0.1f);

	for (int i = 0; i < roomIds.size(); i++)
	{
		if (i >= m_LobbyBtns.size())
		{
			auto& [uuid, btn] = m_LobbyBtns.emplace_back(std::move(std::make_pair(roomIds[i], ButtonComponent(app.GetFont(), app.GetInputHandler()))));
			auto pos = firstButtonPos + sf::Vector2{0.f, BUTTON_LOBBY_SIZE.y * i};

			sf::Color GreenColor(128, 192, 128);
			if (m_LobbyBtns.size() % 2 == 1)
			{
				GreenColor += sf::Color(0, 64, 0);
			}

			btn.SetSize(BUTTON_LOBBY_SIZE);
			btn.SetPosition(pos);
			btn.SetColor(GreenColor);
			app.GetWindow().RegisterDrawable(btn);
		}

		auto& [uuid, lobbyBtn] = m_LobbyBtns[i];
		uuid = roomIds[i];
		lobbyBtn.SetButtonText(std::format("Connect to lobby : {}", uuid));
	}

	m_ReturnButton.Update(deltaTime, app.GetWindow());
	m_CreateButton.Update(deltaTime, app.GetWindow());

	for (auto& [uuid, lobbyBtn] : m_LobbyBtns)
	{
		lobbyBtn.Update(deltaTime, app.GetWindow());
	}

	// Check button pressed

	if (m_ReturnButton.IsPressed())
	{
		m_ClientApp.ChangeState<ConnectionState>(m_ClientApp);
		return;
	}

	if (m_CreateButton.IsPressed())
	{
		Message request(MessageType::RoomCreationRequest);
		auto wrapper = PacketWrapper::Wrap(request);
		wrapper.Sign(m_ClientApp.GetSignature());
		wrapper.Send(m_ClientApp.GetSocket(), m_ClientApp.GetServerAddr());

		return;
	}

	for (auto& [uuid, lobbyBtn] : m_LobbyBtns)
	{
		if (lobbyBtn.IsPressed())
		{
			Message_RoomJoinRequest request(uuid);
			auto wrapper = PacketWrapper::Wrap(request);
			wrapper.Sign(m_ClientApp.GetSignature());
			wrapper.Send(m_ClientApp.GetSocket(), m_ClientApp.GetServerAddr());
			break;
		}
	}
}

void LobbyState::OnExit(ClientApp& app)
{
	app.GetWindow().UnregisterDrawable(m_ReturnButton);
	app.GetWindow().UnregisterDrawable(m_CreateButton);

	for (auto& [uuid, lobbyBtn] : m_LobbyBtns)
	{
		app.GetWindow().UnregisterDrawable(lobbyBtn);
	}

	m_LobbyBtns.clear();
	}

		void LobbyState::InitReturnButton(const sf::Vector2f& pos)
	{
		sf::Color OrangeRed(231, 62, 1);
		std::string btnText = "Return to connection";

		m_ReturnButton.SetSize(BUTTON_SIZE_EXTRA_EXTENDED);
		m_ReturnButton.SetPosition(pos);
		m_ReturnButton.SetColor(OrangeRed);
		m_ReturnButton.SetButtonText(btnText);
	}

	void LobbyState::InitCreateButton(const sf::Vector2f& pos)
	{
		sf::Color GreenColor(62, 231, 1);
		std::string btnText = "Create Room";

		m_CreateButton.SetSize(BUTTON_SIZE_EXTRA_EXTENDED);
		m_CreateButton.SetPosition(pos);
		m_CreateButton.SetColor(GreenColor);
		m_CreateButton.SetButtonText(btnText);
	}

	void LobbyState::JoinLobbyRequest(int lobbyID)
	{
		if (m_IsTryingToJoinLobby) return;

		m_IsTryingToJoinLobby = true;
	}

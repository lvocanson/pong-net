#pragma once
#include <concepts>

enum class MessageType
{
	Unknown = 0,

	// Client -> Server

	Connect,
	Reconnect,
	LobbyListRequest,
	LobbyJoinRequest,
	InputUpdate,

	// Server -> Client

	ConnectResponse,
	LobbyListContent,
	LobbyJoinContent,
	GameUpdate,

	Count // Keep last
};


struct Message
{
	Message(MessageType type = MessageType::Unknown) : type(type) {};
	MessageType type;

	template <std::derived_from<Message> T>
	const T& As() const
	{
		return static_cast<const T&>(*this);
	}
};

struct Message_Connect : public Message
{
	Message_Connect() : Message(MessageType::Connect) {}
};

struct Message_Reconnect : public Message
{
	Message_Reconnect(uint16_t signature)
		: Message(MessageType::Reconnect)
		, signature(signature)
	{
	}
	uint16_t signature;
};

struct Message_ConnectResponse : public Message
{
	Message_ConnectResponse(uint16_t signature)
		: Message(MessageType::ConnectResponse)
		, signature(signature)
	{
	}
	uint16_t signature;
};

struct Message_LobbyListRequest : public Message
{
	Message_LobbyListRequest() : Message(MessageType::LobbyListRequest) {}
};

struct Message_LobbyJoinRequest : public Message
{
	Message_LobbyJoinRequest() : Message(MessageType::LobbyJoinRequest) {}
};

struct Message_InputUpdate : public Message
{
	Message_InputUpdate() : Message(MessageType::InputUpdate) {}
};

struct Message_LobbyListContent : public Message
{
	Message_LobbyListContent() : Message(MessageType::LobbyListContent) {}
};

struct Message_LobbyJoinContent : public Message
{
	Message_LobbyJoinContent() : Message(MessageType::LobbyJoinContent) {}
};

struct Message_GameUpdate : public Message
{
	Message_GameUpdate() : Message(MessageType::GameUpdate) {}
};

#pragma once

enum class MessageType
{
	Unknown = 0,

	Ping,
	PingResponse,

	// Client -> Server

	LobbyListRequest,
	LobbyJoinRequest,
	InputUpdate,

	// Server -> Client

	LobbyListContent,
	LobbyJoinContent,
	GameUpdate,

	Count // Keep last
};


struct Message
{
	Message(MessageType type = MessageType::Unknown) : type(type) {};
	MessageType type;
};

struct Message_Ping : public Message
{
	Message_Ping() : Message(MessageType::Ping) {}
};

struct Message_PingResponse : public Message
{
	Message_PingResponse() : Message(MessageType::PingResponse) {}
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

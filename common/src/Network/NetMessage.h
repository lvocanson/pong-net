#pragma once
#include <cstdint>

enum class NetMessageType : uint32_t
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

template <NetMessageType T>
struct NetMessage 
{
	static_assert("Usage of undefined message type");
};

struct NetHeader
{
	template <NetMessageType T>
	static NetHeader For() { return NetHeader(T, sizeof(NetMessage<T>)); }
	NetHeader();
	NetHeader(NetMessageType, int size);

	bool IsValid() const;

	uint32_t signature;
	NetMessageType type;
	int size;
};

#include "NetMessage.inl"

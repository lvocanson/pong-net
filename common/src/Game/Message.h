#pragma once
#include "Pong.h"
#include <array>
#include <concepts>

enum class MessageType
{
	Unknown = 0,

	// Client -> Server

	Connect,
	QuickMatchRequest,
	RoomCreationRequest,
	RoomGroupRequest,
	RoomJoinRequest,

	// Server -> Client

	ConnectResponse,
	RoomGroupResponse,
	RoomJoinResponse,

	// Both ways

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

struct Message_ConnectResponse : public Message
{
	Message_ConnectResponse(uint16_t signature)
		: Message(MessageType::ConnectResponse)
		, signature(signature)
	{
	}
	uint16_t signature;
};

struct Message_RoomGroupRequest : public Message
{
	Message_RoomGroupRequest(size_t index)
		: Message(MessageType::RoomGroupRequest)
		, index(index)
	{
	}
	size_t index;
};

struct Message_RoomJoinRequest : public Message
{
	Message_RoomJoinRequest(uint16_t uuid)
		: Message(MessageType::RoomJoinRequest)
		, uuid(uuid)
	{
	}

	uint16_t uuid;
};

struct Message_RoomGroupResponse : public Message
{
	Message_RoomGroupResponse(size_t grpIdx, size_t maxGrpIdx)
		: Message(MessageType::RoomGroupResponse)
		, grpIdx(grpIdx), maxGrpIdx(maxGrpIdx)
		, group{}
	{
	}
	size_t grpIdx, maxGrpIdx;

	static constexpr size_t PacketSize = 512;
	static constexpr size_t GroupSize = (PacketSize - 32) / sizeof(uint16_t);
	std::array<uint16_t, GroupSize> group;
};

struct Message_RoomJoinResponse : public Message
{
	enum JoinStatus
	{
		Rejected = 0,
		Accepted,
	};

	Message_RoomJoinResponse(JoinStatus status, uint16_t uuid)
		: Message(MessageType::RoomJoinResponse)
		, status(status)
		, uuid(uuid)
	{
	}

	JoinStatus status;
	uint16_t uuid;
};

struct Message_GameUpdate : public Message
{
	enum GameStatus
	{
		Paused = 0,
		Playing,
	};

	Message_GameUpdate(const Pong& game, GameStatus status, uint16_t leftScore, uint16_t rightScore)
		: Message(MessageType::GameUpdate)
		, game(game), status(status)
		, leftScore(leftScore), rightScore(rightScore)
	{
	}

	Pong game;
	GameStatus status;
	uint16_t leftScore, rightScore;
};

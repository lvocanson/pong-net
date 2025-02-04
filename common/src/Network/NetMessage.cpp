#include "NetMessage.h"
#include "Game/Pong.h"
#include <type_traits>

inline constexpr uint32_t HeaderSignature = 0xADACD892;

NetHeader::NetHeader()
	/* uninitialized */
{
}

NetHeader::NetHeader(NetMessageType type, int size)
	: signature(HeaderSignature)
	, type(type)
	, size(size)
{
}

bool NetHeader::IsValid() const
{
	if (signature != HeaderSignature)
		return false;

	using UType = std::underlying_type_t<NetMessageType>;
	using enum NetMessageType;

#define VALID_CASE(type) case type: return sizeof(NetMessage<type>) == size;

	switch (type)
	{
		VALID_CASE(Ping);
		VALID_CASE(PingResponse);
		VALID_CASE(LobbyListRequest);
		VALID_CASE(LobbyJoinRequest);
		VALID_CASE(InputUpdate);
		VALID_CASE(LobbyListContent);
		VALID_CASE(LobbyJoinContent);
		VALID_CASE(GameUpdate);
	}

#undef VALID_CASE

	return false;
}

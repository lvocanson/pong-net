#define NOMINMAX
#include "PacketUnwrapper.h"
#include <cassert>

PacketUnwrapper::PacketUnwrapper(const Packet& packet)
	: m_Signature(packet.header.signature)
	, m_Timestamp(packet.header.timestamp)
	, m_Remaining(packet.header.total)
	, m_Data(std::make_unique<uint8_t[]>(packet.header.total * sizeof(packet.payload)))
{
	assert(packet.IsValid());
	AddPacket(packet);
}

bool PacketUnwrapper::TryAddPacket(const Packet& packet)
{
	assert(packet.IsValid());
	assert(!IsComplete());

	if (packet.header.timestamp != m_Timestamp || packet.header.signature != m_Signature)
		return false;

	AddPacket(packet);
	return true;
}

void PacketUnwrapper::AddPacket(const Packet& packet)
{
	size_t index = sizeof(packet.payload) * packet.header.index;
	std::memcpy(m_Data.get() + index, &packet.payload, sizeof(packet.payload));
	--m_Remaining;
}

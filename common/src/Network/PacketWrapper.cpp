#define NOMINMAX
#include "PacketWrapper.h"
#include "UdpSocket.h"
#include "NetHelper.h"

static uint16_t GetNumberOfPacketsToSend(size_t size)
{
	return static_cast<uint16_t>(std::ceil(size / static_cast<double>(sizeof(Packet))));
}

PacketWrapper::PacketWrapper(const uint8_t* source, size_t size)
{
	Packet::timestamp_t now = std::chrono::high_resolution_clock::now();
	uint16_t packetCount = GetNumberOfPacketsToSend(size);
	m_Packets.resize(packetCount);

	for (uint16_t i = 0; i < packetCount; ++i)
	{
		auto& packet = m_Packets[i];
		packet.header.timestamp = now;
		packet.header.index = i;
		packet.header.total = packetCount;

		size_t copySize = std::min(sizeof(packet.payload), size);
		std::memcpy(packet.payload, source, copySize);
		source += sizeof(packet.payload);
		size -= copySize;
	}
}

void PacketWrapper::Sign(uint16_t signature)
{
	for (auto& packet : m_Packets)
	{
		packet.Sign(signature);
	}
}

bool PacketWrapper::Send(const UdpSocket& from, const IpAddress& to) const
{
	for (auto& packet : m_Packets)
	{
		if (!from.Send(packet, to))
			return false;
	}
	return true;
}

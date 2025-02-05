#pragma once
#include "Packet.h"
#include <vector>

struct UdpSocket;
struct IpAddress;

class PacketWrapper
{
public:

	template <typename T>
	static PacketWrapper Wrap(const T& object)
	{
		return PacketWrapper(reinterpret_cast<const uint8_t*>(&object), sizeof(T));
	}

	PacketWrapper(const uint8_t* source, size_t size);
	void Sign(uint16_t signature);
	bool Send(const UdpSocket& from, const IpAddress& to) const;

private:

	std::vector<Packet> m_Packets;
};

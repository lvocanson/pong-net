#pragma once
#include "Packet.h"
#include <memory>

class PacketUnwrapper
{
public:

	PacketUnwrapper(const Packet& packet);

	bool TryAddPacket(const Packet& packet);
	bool IsComplete() const { return m_Remaining == 0; }

	Packet::timestamp_t Timestamp() const { return m_Timestamp; }
	uint16_t Signature() const { return m_Signature; }

	template <typename T>
	const T& Unwrap() const
	{
		return *reinterpret_cast<const T*>(m_Data.get());
	}

private:

	void AddPacket(const Packet& packet);

private:

	Packet::timestamp_t m_Timestamp;
	uint16_t m_Signature;
	uint16_t m_Remaining;
	std::unique_ptr<uint8_t[]> m_Data;
};

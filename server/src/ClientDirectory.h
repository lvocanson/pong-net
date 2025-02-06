#pragma once
#include <chrono>
#include <Network/IpAddress.h>
#include <swap_back_array.h>

struct Client
{
	using TimePoint = std::chrono::high_resolution_clock::time_point;

	//char name[22];
	uint16_t signature;
	TimePoint lastPacketSent;
	IpAddress address;
};

class ClientDirectory
{
public:

	const Client& AddOrUpdate(const Client&);
	const Client* FindBySignature(uint16_t) const;
	bool RemoveBySignature(uint16_t);
	
	stc::swap_back_array<Client> m_Directory;
};

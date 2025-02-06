#pragma once
#include <chrono>
#include <Network/IpAddress.h>
#include <swap_back_array.h>

struct Client
{
	using lastContact_t = std::chrono::high_resolution_clock::time_point;

	//char name[22];
	uint16_t signature;
	lastContact_t lastContact = std::chrono::high_resolution_clock::now();
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

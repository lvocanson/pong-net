#pragma once
#include <chrono>
#include <Network/IpAddress.h>

struct Client
{
	using lastContact_t = std::chrono::high_resolution_clock::time_point;

	uint16_t signature;
	IpAddress address;
	lastContact_t lastContact = std::chrono::high_resolution_clock::now();
};

class ClientDirectory
{
public:

	void AddOrUpdate(const Client&);
	const Client* FindBySignature(uint16_t) const;
	bool RemoveBySignature(uint16_t);
	size_t RemoveIfLastContactBefore(Client::lastContact_t);

private:

	std::vector<Client> m_Directory;
};

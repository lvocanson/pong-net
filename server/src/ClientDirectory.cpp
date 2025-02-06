#include "ClientDirectory.h"

const Client& ClientDirectory::AddOrUpdate(const Client& client)
{
	for (auto& c : m_Directory)
	{
		if (c.signature == client.signature)
		{
			c = client;
			return c;
		}
	}

	return m_Directory.emplace_back(client);
}

const Client* ClientDirectory::FindBySignature(uint16_t sig) const
{
	for (auto& c : m_Directory)
	{
		if (c.signature == sig)
		{
			return &c;
		}
	}
	return nullptr;
}

bool ClientDirectory::RemoveBySignature(uint16_t sig)
{
	for (auto it = m_Directory.begin(); it != m_Directory.end(); ++it)
	{
		if (it->signature != sig)
			continue;

		m_Directory.erase_swap(it);
		return true;
	}

	return false;
}

#include "ClientDirectory.h"

void ClientDirectory::AddOrUpdate(const Client& client)
{
	for (auto& c : m_Directory)
	{
		if (c.signature == client.signature)
		{
			c = client;
			return;
		}
	}

	m_Directory.emplace_back(client);
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

		if (std::next(it) != m_Directory.end())
		{
			*it = std::move(m_Directory.back());
		}
		m_Directory.pop_back();
		return true;
	}

	return false;
}

size_t ClientDirectory::RemoveIfLastContactBefore(Client::lastContact_t timestamp)
{
	size_t count = 0;
	for (auto it = m_Directory.begin(); it != m_Directory.end();)
	{
		if (it->lastContact > timestamp)
		{
			++it;
			continue;
		}

		if (std::next(it) != m_Directory.end())
		{
			*it = std::move(m_Directory.back());
		}
		m_Directory.pop_back();
		++count;
	}
	return count;
}

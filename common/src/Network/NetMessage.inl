#pragma once
#include "NetMessage.h"
#include <cassert>
#include <vector>

template <>
struct NetMessage<NetMessageType::Unknown>
{
	char* GetBufferOfSize(size_t size)
	{
		data.resize(size);
		return data.data();
	}

	template <NetMessageType T>
	NetMessage<T> Convert() const
	{
		assert(data.size() == sizeof(NetMessage<T>));
		NetMessage<T> retval;
		std::memcpy(&retval, data.data(), sizeof(NetMessage<T>));
		return retval;
	}

	std::vector<char> data;
};

template <>
struct NetMessage<NetMessageType::Ping> {};

template <>
struct NetMessage<NetMessageType::PingResponse> {};

/// Client -> Server


/// Server -> Client

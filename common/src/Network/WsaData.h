#pragma once
#include <WinSock2.h>

struct WsaData
{
	WsaData();
	~WsaData();

	operator WSADATA& () { return data; }
	WSADATA* operator&() { return &data; }

	WSADATA data;
	int error;
};

#include "WsaData.h"

WsaData::WsaData()
	: data{}
	, error(WSAStartup(MAKEWORD(2, 2), &data))
{
}

WsaData::~WsaData()
{
	if (!error)
		WSACleanup();
}

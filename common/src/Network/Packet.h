#pragma once
#include <chrono>

struct Packet
{
	void Sign(uint16_t signature);
	bool IsValid() const;

	using timestamp_t = std::chrono::high_resolution_clock::time_point;
	struct
	{
		timestamp_t timestamp;
		uint16_t secret;
		uint16_t signature;
		uint16_t index;
		uint16_t total;

	} header;
	uint8_t payload[512 - sizeof(header)];
};

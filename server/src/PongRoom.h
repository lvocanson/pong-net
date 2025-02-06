#pragma once
#include "Game/Pong.h"
#include <cstdint>
#include <chrono>

struct PongRoom
{
	PongRoom(uint16_t uuid);

	bool IsFull() const { return !ContainsSignature(0); }
	bool ContainsSignature(uint16_t sig) const { return leftSignature == sig || rightSignature == sig; }

	std::chrono::high_resolution_clock::time_point leftLastUpdate, rightLastUpdate;
	uint16_t leftSignature, rightSignature;

	uint16_t leftScore, rightScore;
	Pong game;

	uint16_t uuid;
};

#include "PongRoom.h"

PongRoom::PongRoom(uint16_t uuid)
	: leftLastUpdate(std::chrono::high_resolution_clock::time_point::max())
	, rightLastUpdate(std::chrono::high_resolution_clock::time_point::max())
	, leftSignature(0), rightSignature(0)
	, leftScore(0), rightScore(0)
	, game()
	, uuid(uuid)
{
	game.Reset();
}

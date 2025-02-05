#include "PongRoom.h"

PongRoom::PongRoom()
	: leftLastUpdate(std::chrono::high_resolution_clock::time_point::min())
	, rightLastUpdate(std::chrono::high_resolution_clock::time_point::min())
	, leftSignature(0), rightSignature(0)
	, leftScore(0), rightScore(0)
	, game()
{
	game.Reset();
}

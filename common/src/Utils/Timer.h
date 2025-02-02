#pragma once
#include <chrono>

class Timer
{
public:

	using hrc = std::chrono::high_resolution_clock;

	Timer();

	float GetElapsedSeconds();
	void Restart();

private:

	hrc::time_point m_Start;
};

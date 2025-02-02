#include "timer.h"

Timer::Timer()
	: m_Start(hrc::now())
{
}

float Timer::GetElapsedSeconds()
{
	return std::chrono::duration<float>(hrc::now() - m_Start).count();
}

void Timer::Restart()
{
	m_Start = hrc::now();
}

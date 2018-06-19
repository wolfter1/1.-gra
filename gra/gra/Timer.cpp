#include "Timer.h"



Timer::Timer()
{
	time.restart();
	bPause = false;
	runTime = 0;
}
Timer::~Timer()
{
}
void Timer::start()
{
	if (bPause)
		time.restart();
	bPause = false;
}

void Timer::restart()
{
	time.restart();
	bPause = false;
	runTime = 0;
}
void Timer::pause()
{
	if(!bPause)
		runTime += time.getElapsedTime().asSeconds();
	bPause = true;
}
float Timer::getTime()
{
	if (!bPause)
		return runTime + time.getElapsedTime().asSeconds();
	else
		return runTime;
}
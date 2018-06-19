#pragma once
#include "include.h"
class Timer
{
	sf::Clock time;
	float runTime;
	bool bPause;

public:
	Timer();
	~Timer();
	void start();
	void restart();
	void pause();
	float getTime();
};


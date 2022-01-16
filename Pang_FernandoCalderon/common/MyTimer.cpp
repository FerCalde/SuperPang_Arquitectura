#include "MyTimer.h"
//
#include <iostream> //BORRAR


MyTimer::MyTimer()
{
	PCFreq = 0.f;
	performanceCounterStart = 0;
	currentCounterFPS = 0.f;
	realFPS = 0.f;
	totalFPS = 0.f;
	elapsedTime = 0.f;
	previousTime = 0.f;
	currentTime = 0.f;
	currentTimeByFixedTicks = 0;
	fixedTick = 1.f / 60.f;
	espiralTime = (1.f / 15.f);
	logicCurrentTime = 0.f;
}

MyTimer::~MyTimer()
{

}

void MyTimer::StartCounter()
{
	LARGE_INTEGER currentPerformanceCounter;
	if (!QueryPerformanceFrequency(&currentPerformanceCounter))
	{
		std::cout << "QueryPerformanceFrequency failed!\n";
	}

	PCFreq = float(currentPerformanceCounter.QuadPart);

	QueryPerformanceCounter(&currentPerformanceCounter);
	performanceCounterStart = currentPerformanceCounter.QuadPart;

	previousTime = GetTime();
}

float MyTimer::GetTime()
{
	LARGE_INTEGER currentPerformanceCounter;
	QueryPerformanceCounter(&currentPerformanceCounter);
	return float(currentPerformanceCounter.QuadPart - performanceCounterStart) / PCFreq;
}

inline void MyTimer::AddFPS()
{
	currentCounterFPS++;
}

float MyTimer::GetFPSRate()
{
	return currentCounterFPS / GetTime();
}

float MyTimer::GetFixedTick()
{
	return fixedTick;
}

float MyTimer::GetLogicCurrentTime()
{
	return logicCurrentTime;
}

void MyTimer::InitSlotsToProcess()
{
	currentTime = GetTime();
	elapsedTime += currentTime - previousTime;
	previousTime = currentTime;

	currentTimeByFixedTicks += fixedTick;

	if (currentTimeByFixedTicks - currentCounterFPS >= 1.f)
	{
		totalFPS = currentCounterFPS;
		currentCounterFPS = 0;
		realFPS++;
	}
}

bool MyTimer::ProcessSlots()
{
	return elapsedTime >= fixedTick ? true : false;
}

void MyTimer::ElapsedFixedTick()
{
	if (elapsedTime >= (1.f / 15.f))
	{
		elapsedTime = fixedTick;
	}
	else
	{
		elapsedTime -= fixedTick;
	}

	logicCurrentTime += fixedTick;
	AddFPS();

}
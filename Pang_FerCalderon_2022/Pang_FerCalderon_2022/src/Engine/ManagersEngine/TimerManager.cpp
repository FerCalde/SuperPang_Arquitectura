#include "TimerManager.h"
//


MyTimerManager::MyTimerManager()
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

MyTimerManager::~MyTimerManager()
{

}

void MyTimerManager::StartCounter()
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

float MyTimerManager::GetTime()
{
	LARGE_INTEGER currentPerformanceCounter;
	QueryPerformanceCounter(&currentPerformanceCounter);
	return float(currentPerformanceCounter.QuadPart - performanceCounterStart) / PCFreq;
}

inline void MyTimerManager::AddFPS()
{
	currentCounterFPS++;
}

float MyTimerManager::GetFPSRate()
{
	return currentCounterFPS / GetTime();
}

float MyTimerManager::GetFixedTick()
{
	return fixedTick;
}

float MyTimerManager::GetLogicCurrentTime()
{
	return logicCurrentTime;
}

void MyTimerManager::InitSlotsToProcess()
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

bool MyTimerManager::ProcessSlots()
{
	return elapsedTime >= fixedTick ? true : false;
}

void MyTimerManager::ElapsedFixedTick()
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
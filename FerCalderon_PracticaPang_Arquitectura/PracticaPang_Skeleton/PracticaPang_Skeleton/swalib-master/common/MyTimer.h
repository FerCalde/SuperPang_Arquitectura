#pragma once
#include <windows.h>



class MyTimer
{
public:

	MyTimer();
	~MyTimer();

	void StartCounter();
	float GetTime();
	void AddFPS();
	float GetFPSRate();
	float GetFixedTick();
	float GetLogicCurrentTime();

	void InitSlotsToProcess();
	bool ProcessSlots();


	void ElapsedFixedTick();

private:

	float PCFreq = 0.f;
	__int64 performanceCounterStart = 0;
	float currentCounterFPS = 0.f;
	float realFPS = 0.f;
	float elapsedTime = 0.f;
	float previousTime = 0.f;
	float currentTime = 0.f;
	float currentTimeByFixedTicks = 0.f;
	float totalFPS = 0.f;
	float fixedTick = 1.f / 60.f;
	float espiralTime = (1.f / 15.f);
	float logicCurrentTime = 0.f;


};

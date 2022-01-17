#pragma once

#include "example.h"
#include <iostream> //BORRAR
#include "InputManager.h"

int Main(void)
{
	MyTimer* ptrTimerManager = new MyTimer();
	ptrTimerManager->StartCounter();

	CInputManager::GetInstance().CreateSingleton();
	LogicManager* ptrLogicManager = LogicManager::GetInstance();
	RenderEngine* ptrRenderManager = RenderEngine::GetInstance();

	
	ptrLogicManager->InitLogic();
	ptrRenderManager->InitRender();

	while (!SYS_GottaQuit()) // Controlling a request to terminate an application.
	{

		ptrLogicManager->LogicSlot(*ptrTimerManager);

		ptrRenderManager->RenderSlot( ptrTimerManager->GetFPSRate(), ptrTimerManager->GetLogicCurrentTime(), ptrTimerManager->GetTime());
	}

	// End app.
	ptrLogicManager->ShutdownLogic();
	ptrRenderManager->ShutdownRender();

	delete ptrTimerManager;
	ptrTimerManager = nullptr;


	return 0;
}

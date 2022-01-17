#include "LogicManager.h"
#include "MyTimer.h"
#include <iostream>
#include "stdafx.h"
#include "sys.h"
#include "core.h"

//Forward Declaration de m_instance 
LogicManager* LogicManager::m_instance = nullptr;

void LogicManager::InitLogic()
{
	LogicManager* instanceLogicManager = LogicManager::GetInstance();
	LoadTextures();
	Entity* auxNewEntity = nullptr;


#pragma region Create_GameEntities
#pragma region Create_Player
	auxNewEntity = new Player();
	auxNewEntity->SetID(instanceLogicManager->id);


	vec2 auxPos(SCR_WIDTH*0.5f,  20);
	vec2 auxVel(100,0);
	float auxRadius(16.f);

	auxNewEntity->FindComponent<CMP_Transform>()->SetPos(auxPos);
	auxNewEntity->FindComponent<CMP_Transform>()->SetVel(auxVel);

	auxNewEntity->FindComponent<CMP_Collider>()->SetRadius(25.f);
	auxNewEntity->FindComponent<CMP_Render>()->SetGfxSprite(texPlayer);




	auxNewEntity->ActivateEntity();
	instanceLogicManager->m_entitiesList.push_back(auxNewEntity);
	instanceLogicManager->id++;
	auxNewEntity = nullptr;
#pragma endregion

#pragma region Create_Bullets
	for (unsigned int i = 0; i < NUM_BULLETS; i++)
	{
	auxNewEntity = new Bullet();
	vec2 auxPos(0,0);
	vec2 auxVel(0, MAX_BULLET_SPEED);
	float auxRadius(16.f);


	auxNewEntity->FindComponent<CMP_Transform>()->SetPos(auxPos);
	auxNewEntity->FindComponent<CMP_Transform>()->SetVel(auxVel);
	auxNewEntity->FindComponent<CMP_Collider>()->SetRadius(16.f);

	auxNewEntity->FindComponent<CMP_Render>()->SetGfxSprite(texBullet);

	auxNewEntity->SetID(instanceLogicManager->id);


	auxNewEntity->DesactivateEntity();

	instanceLogicManager->m_entitiesList.push_back(auxNewEntity);
	instanceLogicManager->id++;

	auxNewEntity = nullptr;

	auxNewEntity = nullptr;
	}
#pragma endregion

#pragma region Create_Balls
	for (unsigned int i = 0; i < NUM_BALLS ; i++)
	{
		auxNewEntity = new Ball();

		vec2 auxPos(CORE_FRand(0.0, SCR_WIDTH), CORE_FRand(0.0, SCR_HEIGHT));
		vec2 auxVel(vec2(CORE_FRand(-MAX_BALL_SPEED, +MAX_BALL_SPEED) * 10, CORE_FRand(-MAX_BALL_SPEED, +MAX_BALL_SPEED) * 10));
		float auxRadius(16.f);


		//auxNewEntity->SendMsg();
		auxNewEntity->FindComponent<CMP_Transform>()->SetPos(auxPos);
		auxNewEntity->FindComponent<CMP_Transform>()->SetVel(auxVel);
		auxNewEntity->FindComponent<CMP_Collider>()->SetRadius(16.f);

		auxNewEntity->FindComponent<CMP_Render>()->SetGfxSprite(texsmallball);

		auxNewEntity->SetID(instanceLogicManager->id);


		auxNewEntity->ActivateEntity();

		instanceLogicManager->m_entitiesList.push_back(auxNewEntity);
		instanceLogicManager->id++;
		auxNewEntity = nullptr;

	}



	auxNewEntity = nullptr;

#pragma endregion
#pragma endregion
}

void LogicManager::ShutdownLogic()
{

	for (auto& _currentEntity : m_entitiesList)
	{
		delete _currentEntity;
		_currentEntity = nullptr;
	}
	m_entitiesList.clear();

	ShutdownTextures();
}


void LogicManager::LoadTextures()
{
	texbkg = CORE_LoadPNG("../../data/circle-bkg-128.png", true);
	texbkg2 = CORE_LoadPNG("../../data/backgroundMap.png", true);
	texsmallball = CORE_LoadPNG("../../data/tyrian_ball.png", false);
	texPlayer = CORE_LoadPNG("../../data/Player.png", false);
	texBullet = CORE_LoadPNG("../../data/ball128.png", false);
}

void LogicManager::ShutdownTextures()
{
	CORE_UnloadPNG(texbkg);
	CORE_UnloadPNG(texsmallball);
	CORE_UnloadPNG(texPlayer);
}



void LogicManager::LogicSlot(MyTimer& _timerManager)
{
	_timerManager.InitSlotsToProcess();

	while (_timerManager.ProcessSlots()) // si elapsedTime es mayor o igual que fixedTick..
	{

		LogicWorldSlot(_timerManager.GetFixedTick());

		_timerManager.ElapsedFixedTick();
		//SYS_Sleep(16);
	}
}

void LogicManager::LogicWorldSlot(float _fFixedTick)
{
	for (auto& entity : m_entitiesList)
	{
		if(entity->IsActive())
		{
			entity->Slot(_fFixedTick);	
		}
	}
}


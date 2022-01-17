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

	m_GameManager = new CGameManagerInfo();
	LoadTextures();


	Entity* auxNewEntity = nullptr;

#pragma region Create_GameEntities

#pragma region Create_Player
	auxNewEntity = new Player();
	auxNewEntity->SetID(instanceLogicManager->id);


	vec2 auxPos(SCR_WIDTH * 0.5f, 20);
	vec2 auxVel(200, 0);
	float auxRadius(20.f);



	auxNewEntity->FindComponent<CMP_Transform>()->SetPos(auxPos);
	auxNewEntity->FindComponent<CMP_Transform>()->SetVel(auxVel);

	auxNewEntity->FindComponent<CMP_Collider>()->SetRadius(auxRadius);
	auxNewEntity->FindComponent<CMP_Render>()->SetGfxSprite(texPlayer);

	auxNewEntity->FindComponent<CMP_SpawnerEntity>()->SetTimeFireSpawnMax(0.3f);


	auxNewEntity->ActivateEntity();
	instanceLogicManager->m_entitiesList.push_back(auxNewEntity);
	instanceLogicManager->id++;
	GetGameManager()->SetPlayerLifes(auxNewEntity->FindComponent<CMP_LifePlayer>()->GetLife());
	auxNewEntity = nullptr;
#pragma endregion

#pragma region Create_Bullets
	for (unsigned int i = 0; i < NUM_BULLETS; i++)
	{
		auxNewEntity = new Bullet();
		vec2 auxPos(0, 0);
		vec2 auxVel(0, MAX_BULLET_SPEED);
		float auxRadius(10.f);


		auxNewEntity->FindComponent<CMP_Transform>()->SetPos(auxPos);
		auxNewEntity->FindComponent<CMP_Transform>()->SetVel(auxVel);
		auxNewEntity->FindComponent<CMP_Collider>()->SetRadius(auxRadius);

		auxNewEntity->FindComponent<CMP_Render>()->SetGfxSprite(texBullet);

		auxNewEntity->SetID(instanceLogicManager->id);


		auxNewEntity->DesactivateEntity();

		instanceLogicManager->m_entitiesList.push_back(auxNewEntity);
		instanceLogicManager->id++;

		auxNewEntity = nullptr;

	}
#pragma endregion

#pragma region Create_Balls
	//srand(time(NULL));

	int offsetBallsX = 60;
	int offsetBallY = 80;
	const int ROWS = 3;
	const int COLUMS = 3;
	vec2 posBallSpawn(SCR_WIDTH, SCR_HEIGHT);
	int cantSpawned = 0;

	//for (unsigned int k = 0; k < NUM_BALLS; k++)
	{
		for(int i=0; i<=ROWS; i++)
		{
			posBallSpawn.y-= offsetBallY;

			for(int j=0; j<=COLUMS; j++)
			{
				posBallSpawn.x -= offsetBallsX;
				
				if (cantSpawned < NUM_BALLS)
				{
					auxNewEntity = new Ball();

					//vec2 auxPos(p);
					//vec2 auxPos(CORE_FRand(0.0, SCR_WIDTH), CORE_FRand(SCR_HEIGHT * 0.5f, SCR_HEIGHT));


					vec2 auxVel(vec2(CORE_FRand(-MAX_BALL_SPEED, +MAX_BALL_SPEED) * 10, CORE_FRand(-MAX_BALL_SPEED, +MAX_BALL_SPEED) * 10));
					float auxRadius(20.f);


					//auxNewEntity->SendMsg();
					//auxNewEntity->FindComponent<CMP_Transform>()->SetPos(auxPos);
					auxNewEntity->FindComponent<CMP_Transform>()->SetPos(posBallSpawn);
					auxNewEntity->FindComponent<CMP_Transform>()->SetVel(auxVel);
					auxNewEntity->FindComponent<CMP_Collider>()->SetRadius(auxRadius);

					auxNewEntity->FindComponent<CMP_Render>()->SetGfxSprite(texsmallball);

					auxNewEntity->SetID(instanceLogicManager->id);


					auxNewEntity->ActivateEntity();

					instanceLogicManager->m_entitiesList.push_back(auxNewEntity);
					instanceLogicManager->id++;
					auxNewEntity = nullptr;
					cantSpawned++;
				}
				
			}
			
		}

		

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
		if (entity->IsActive())
		{
			entity->Slot(_fFixedTick);
		}
	}
}

Entity* LogicManager::FindGameObjectOfTag(Entity::ETagEntity _tagFinder)
{
	for (Entity* currentEntity : m_entitiesList)
	{
		if (!currentEntity->IsActive())
		{

			return (currentEntity->HasTag(_tagFinder)) ? currentEntity : nullptr;
			break;
		}

	}
	return nullptr;

	return nullptr;
}


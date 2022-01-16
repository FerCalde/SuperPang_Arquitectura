#include "..\..\Juego1D_FerCalderon_2022.h"
#include "LogicManager.h"

//Forward Declaration de m_instance 
//LogicManager* LogicManager::m_instance = nullptr;

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
}

void LogicManager::InitLogic()
{
	InitGameObjects();
	m_GameManager = new CGameManagerInfo;
}

void LogicManager::InitGameObjects()
{
	int iEntityID = 0;

	Entity* auxNewEntity = nullptr;

#pragma region CREATE PLAYER
	//INIT PLAYER ENTITY
	auxNewEntity = new Player();
	auxNewEntity->SetID(iEntityID);

	auxNewEntity->AddComponent(new CMP_Transform);//SIEMPRE LO PRIMERO!
	auxNewEntity->AddComponent(new CMP_Collider);
	auxNewEntity->AddComponent(new CMP_InputController);
	auxNewEntity->AddComponent(new CMP_Render);
	auxNewEntity->AddComponent(new CMP_Shooter);
	//Set Initial Vars
	float auxPos = RenderEngine::GetInstance().WEIGHT_MAP * 0.5f;
	char auxSymbol = 'X';

	auxNewEntity->FindComponent<CMP_Transform>()->SetPos(auxPos);
	auxNewEntity->FindComponent<CMP_Render>()->SetSymbol(auxSymbol);
	auxNewEntity->SetTag(Entity::ETagEntity::Player);

	auxNewEntity->ActivateEntity();

	m_entitiesList.push_back(auxNewEntity);
	iEntityID++;
#pragma endregion

#pragma region CREATE ENEMIES

	//INIT ENEMIES ENTITIES
	for (int i = 0; i < MAX_ENEMIES; i++)
	{
		auxNewEntity = new CEnemy();
		auxNewEntity->AddComponent(new CMP_Transform);
		auxNewEntity->AddComponent(new CMP_Collider);
		auxNewEntity->AddComponent(new CMP_Render);
		//Set Initial Vars
		auxSymbol = '*';

		auxNewEntity->FindComponent<CMP_Render>()->SetSymbol(auxSymbol);
		auxNewEntity->SetID(iEntityID);
		auxNewEntity->SetTag(Entity::ETagEntity::Enemy);

		auxNewEntity->DesactivateEntity();

		m_entitiesList.push_back(auxNewEntity);
		iEntityID++;

	}

#pragma endregion

#pragma region CREATE BULLETS
	//INIT BULLETS ENTITIES
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		auxNewEntity = new CBullet();
		auxNewEntity->AddComponent(new CMP_Transform);
		auxNewEntity->AddComponent(new CMP_Collider);
		auxNewEntity->AddComponent(new CMP_Render);

		auxNewEntity->SetID(iEntityID);
		auxNewEntity->SetTag(Entity::ETagEntity::Bullet);
		auxNewEntity->DesactivateEntity();

		m_entitiesList.push_back(auxNewEntity);
		iEntityID++;
	}
#pragma endregion
}

void LogicManager::LogicSlot(MyTimerManager& _timerManager)
{
	_timerManager.InitSlotsToProcess();

	while (_timerManager.ProcessSlots()) // si elapsedTime es mayor o igual que fixedTick..
	{

		LogicWorldSlot(_timerManager.GetFixedTick());

		_timerManager.ElapsedFixedTick();
		//SYS_Sleep(16);
	}
}

void LogicManager::LogicWorldSlot(const float& _fFixedTick)
{
	RenderEngine::GetInstance().GetMap()->SetCleanMap(); //Seteo el mapa limpio para reiniciar las posiciones que eran ocupadas por entidades

	for (auto& entity : m_entitiesList)
	{
		if (entity->IsActive())
		{
			entity->Slot(_fFixedTick);

			UpdateMapEntityPositions(entity); //Comunicacion LogicEntityPosition con RenderMap
		}

	}

	SpawnEnemy(_fFixedTick);

}

void LogicManager::UpdateMapEntityPositions(Entity*& currentEntity)
{
	if (currentEntity->IsActive()) //En caso de estar activado
	{
		if (currentEntity->FindComponent<CMP_Render>() != NULL) //En caso de tener un componente Render
		{

			int auxEntityPos = currentEntity->FindComponent<CMP_Transform>()->GetPos().x; //La posicion del entity guardado para sobreescribir su index en el mapa
			RenderEngine::GetInstance().GetMap()->ptrMyBoard[auxEntityPos] = currentEntity->FindComponent<CMP_Render>()->GetSymbol(); //Cargo el simbolo de la entidad en la posicion que tenga en el mundo

		}
	}

}

void LogicManager::ShutdownLogic()
{

	for (auto& _currentEntity : m_entitiesList)
	{
		delete _currentEntity;
		_currentEntity = nullptr;
	}
	m_entitiesList.clear();

	delete m_GameManager;
	m_GameManager = nullptr;
	/*
	ShutdownTextures();*/
}

void LogicManager::SpawnEnemy(const float& _fFixedTick)
{
	m_TimeSpawn += _fFixedTick;
	if (m_TimeSpawn >= m_TimeSpawn_MAX)
	{
		
		if (m_TimeSpawn_MAX > 0)
		{
			float auxFixed = _fFixedTick * 2.f;
			m_TimeSpawn_MAX -= auxFixed;
		}
		
		//PUEDO CAMBIAR ESTE CODIGO POR LA FUNC FINDGAMEOBJECTOFTAG
		for (size_t i = 0; i < LogicManager::GetInstance().m_entitiesList.size(); i++)
		{
			if (LogicManager::GetInstance().m_entitiesList[i]->HasTag(Entity::ETagEntity::Enemy))
			{
				//AUMENTO LA VELOCIDAD DE LAS ENTIDADES 
				float auxFixed = _fFixedTick * 75.f;
				float auxSpeedAument = LogicManager::GetInstance().m_entitiesList[i]->FindComponent<CMP_Transform>()->GetVel().x + auxFixed;
				LogicManager::GetInstance().m_entitiesList[i]->FindComponent<CMP_Transform>()->SetVelInit(auxSpeedAument);
				
				if (!LogicManager::GetInstance().m_entitiesList[i]->IsActive())
				{
					RandomEnemySide(LogicManager::GetInstance().m_entitiesList[i]);


					break;
				}
			}
		}

		
		m_TimeSpawn = 0;
	}
}

void LogicManager::RandomEnemySide(Entity* _currentEnemyToSpawn)
{

	float auxRandomPos = rand() % 100; //Spawn aleatorio
	if (auxRandomPos < 50) //Lado izquierdo
	{
		_currentEnemyToSpawn->FindComponent<CMP_Transform>()->SetPos(0);
		_currentEnemyToSpawn->FindComponent<CMP_Transform>()->SetMoveDir(1);

	}
	else //Lado derecho
	{
		_currentEnemyToSpawn->FindComponent<CMP_Transform>()->SetPos(RenderEngine::GetInstance().WEIGHT_MAP);
		_currentEnemyToSpawn->FindComponent<CMP_Transform>()->SetMoveDir(-1);
	}

	vec2 auxVel = _currentEnemyToSpawn->FindComponent<CMP_Transform>()->GetVelInit();
	_currentEnemyToSpawn->FindComponent<CMP_Transform>()->SetVel(auxVel);


	_currentEnemyToSpawn->ActivateEntity();

}
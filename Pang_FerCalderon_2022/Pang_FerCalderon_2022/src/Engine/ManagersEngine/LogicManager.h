#pragma once
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include "..\miscellaneous\vector2d.h"
#include "..\..\GameAssets\EnemyScripts\EnemyEntity.h"
#include "..\..\GameAssets\EntityComponentSystem\EntityAndComponent.h"
#include "..\..\GameAssets\PlayerScripts\Bullet.h"
#include "..\..\GameAssets\PlayerScripts\PlayerEntity.h"
#include "ISingleton.h"
#include"RenderManager.h"
#include "TimerManager.h"

struct CGameManagerInfo
{
	CGameManagerInfo() { iEnemiesDead = 0; }
	~CGameManagerInfo(){}
	
	bool isPlayerAlive = true;
	void UpdatePlayerLife(bool _isAlive)
	{
		isPlayerAlive = _isAlive;
	}

	int iEnemiesDead = 0;
	void AddEnemyDead() { iEnemiesDead++; }
	int GetEnemiesDead() { return iEnemiesDead; }




};

class LogicManager : public ISingleton<LogicManager>
{
private:
	//LogicManager() { }
	~LogicManager() {}
	
public:
	CGameManagerInfo* m_GameManager = nullptr;
	int MAX_BULLETS = 5;
	int MAX_ENEMIES = 10;
	std::vector<Entity*> m_entitiesList; // creo el vector de Entidades


	//Spawn Variables
	//A medida que pasa el tiempo y se Spawnean Enemigos, se reduce el tiempo entre spawns para dar dificultad
	float m_TimeSpawn = 0.f;
	float m_TimeSpawn_MAX = 1.f; //2Segundos,  lo calculo por segun los frames que pasan (60fps)

	Entity* FindGameObjectOfTag(Entity::ETagEntity _tagFinder);
	
	void InitGameObjects();
	void InitLogic();

	void LogicSlot(MyTimerManager& _timerManager);
	void LogicWorldSlot(const float& _fFixedTick);

	void SpawnEnemy(const float& _fFixedTick);
	void RandomEnemySide(Entity* currentEnemyToSpawn);
	void UpdateMapEntityPositions(Entity*& currentEntity);
	
	void ShutdownLogic();
	
};








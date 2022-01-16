#pragma once
#include<string>
#include "ISingleton.h"
#include "LogicManager.h"
#include "..\..\GameAssets\EntityComponentSystem\Components.h"
#include "..\miscellaneous/vector2d.h"
#include <iostream>
#include <stdlib.h>

//class CMap;


struct myMap
{
	friend class LogicManager;
	const int ROWS = 30;
	const int COLUMNS = 3;

	char* ptrMyBoard = nullptr;

	myMap()
	{
		InitDefaultMap();
	}
	myMap(const int _mapSize) : ROWS(_mapSize)
	{
		InitDefaultMap();
	}

	void InitDefaultMap()
	{
		ptrMyBoard = new char[ROWS];
		SetCleanMap();
	}
	void SetCleanMap()
	{
		for (int i = 0; i < ROWS; i++)
		{
			ptrMyBoard[i] = '_';
		}
		ptrMyBoard[ROWS - 1] = '\0';
	}

	
	/*char myboard[ROWS][COLUMNS] = { {'O',	'X',	'O'},
									{'K',	'X',	'X'},
									{'X',	'O',	'O'},
	};*/
};




class RenderEngine : public ISingleton<RenderEngine> // SINGLETON
{
private:
	~RenderEngine() { }

public:

	myMap* o_myMap=nullptr;
	
	const int WEIGHT_MAP = 50;

	void InitRender();
	void RenderSlot(const float& _fps, const float& _elapsed, const float& _currentTime);
	void UpdateRenderMapBoard(char* _auxLogicMap_);
	void ShutdownRender();

	inline void ClearScene()
	{
		system("CLS");
	}
	
	myMap* GetMap(){return o_myMap;}
};


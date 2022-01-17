#pragma once
#include<string>
struct UIManager;
class RenderEngine // SINGLETON
{
public:
	static RenderEngine* GetInstance()
	{
		/*if (instance == nullptr)
		{
			RenderEngine* instance = new RenderEngine;
		}*/
		static RenderEngine* instance;
		return instance;
	}

private:
	RenderEngine() {  }
	static RenderEngine* instance;

public:
	

	void InitRender();
	void RenderSlot(float _fps, float _elapsed, float _currentTime);
	void ShutdownRender();
};

struct UIManager
{
	int playerLifesUI = -1;
	int iEnemiesDeadUI = 0;

	void UpdatePlayerLifeUI(const int& _playerLifes) { playerLifesUI = _playerLifes; }
	void AddiEnemiesUI() { iEnemiesDeadUI++; }
	int GetPlayerLifesUI() { return playerLifesUI; }
	int GetiEnemiesDeadUI() { return iEnemiesDeadUI; }
};
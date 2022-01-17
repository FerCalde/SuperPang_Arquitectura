#include <vector>
#include "balls.h"
#include "EntityAndComponent.h"
#include "Player.h"

class LogicManager // SINGLETON
{
public:
	static LogicManager* GetInstance()
	{
		if (m_instance == nullptr)
		{
			m_instance = new LogicManager;
		}
		return m_instance;
	}

private:
	GLuint texsmallball = 0;
	GLuint texbkg = 0;
	GLuint texbkg2 = 0;
	GLuint texPlayer = 0;
	GLuint texBullet = 0;
	const unsigned int NUM_BALLS = 10;	
	const float MAX_BALL_SPEED = 15.f;

	const unsigned int NUM_BULLETS = 5;
	const float MAX_BULLET_SPEED = 150.f;

	size_t id = 0;


	LogicManager() {}
	static LogicManager* m_instance;

public:
	std::vector<Entity*> m_entitiesList; // creo el vector de Entidades

	GLuint GetTexBK() { return texbkg; }
	GLuint GetTexBK2() { return texbkg2; }
	GLuint GetTexSmallBall() { return texsmallball; }
	GLuint GetTexPlayer() { return texPlayer; }
	GLuint GetTexBullet() { return texBullet; }


	void InitLogic();
	void ShutdownLogic();
	void LoadTextures();
	void ShutdownTextures();
	
	void LogicSlot(MyTimer& _timerManager);
	void LogicWorldSlot(float _fFixedTick);
};

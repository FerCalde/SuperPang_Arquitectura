#pragma once
#include<string>

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
	RenderEngine() { }
	static RenderEngine* instance;

public:
	void InitRender();
	void RenderSlot(float _fps, float _elapsed, float _currentTime);
	void ShutdownRender();
};
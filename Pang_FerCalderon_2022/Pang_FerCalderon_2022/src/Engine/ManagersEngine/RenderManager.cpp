
#include "RenderManager.h"


//RenderEngine* RenderEngine::instance = nullptr;

void RenderEngine::InitRender()
{
	o_myMap = new myMap(WEIGHT_MAP);
}

void RenderEngine::ShutdownRender()
{
	delete o_myMap;
	o_myMap = nullptr;
}



void RenderEngine::RenderSlot(const float& _fps, const float& _elapsed, const float& _currentTime)
{
	ClearScene(); //Limpio la pantalla antes de pintar

	std::string mapString(RenderEngine::GetInstance().GetMap()->ptrMyBoard); //Cargo el mapa en una string
	std::cout << mapString << "\tSCORE: " << LogicManager::GetInstance().m_GameManager->GetEnemiesDead()<< "\n";

	
}

void RenderEngine::UpdateRenderMapBoard(char* _auxLogicMap_)
{
	for (int i = 0; i < RenderEngine::GetInstance().WEIGHT_MAP
		; i++)
	{
		RenderEngine::GetInstance().GetMap()->ptrMyBoard[i] = _auxLogicMap_[i]; //Cargo el mapa auxiliar que envia el LogicManager cuando actualiza el mundo.
		
		
	}
}

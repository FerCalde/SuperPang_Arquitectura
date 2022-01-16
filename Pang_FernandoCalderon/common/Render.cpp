#include <string>
#include "LogicManager.h"
#include "render.h"
#include "Components.h"
#include "stdafx.h"
#include "font.h"
#include "vector2d.h"
#include "core.h"
#include "sys.h"


//RenderEngine* RenderEngine::instance = nullptr;

void RenderEngine::InitRender()
{
	FONT_Init();

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT); // Sets up clipping.
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);	// Specifies clear values for the color buffers.
	glMatrixMode(GL_PROJECTION);	// Specifies projection matrix is the current matrix.
	glLoadIdentity();	// Replaces the current matrix with the identity matrix.
	glOrtho(0.0, SCR_WIDTH, 0.0, SCR_HEIGHT, 0.0, 1.0);	// Multiplies the current matrix by an orthographic matrix.
	glEnable(GL_TEXTURE_2D);	// Enabling two-dimensional texturing.
	// NOTA: Mirar diferencias comentando las 2 siguientes funciones.
	glEnable(GL_BLEND);	// Blend the incoming RGBA color values with the values in the color buffers.
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	// Blend func. for alpha color.
}

void RenderEngine::ShutdownRender()
{
	FONT_End();
}


void RenderEngine::RenderSlot(float _fps, float _elapsed, float _currentTime)
{
	LogicManager* instanceLogicManager = LogicManager::GetInstance();
	// Render
	glClear(GL_COLOR_BUFFER_BIT);	// Clear color buffer to preset values.

	// Render backgground
	for (int i = 0; i <= SCR_WIDTH / 128; i++) {
		for (int j = 0; j <= SCR_HEIGHT / 128; j++) {
			CORE_RenderCenteredSprite(vec2(i * 128.f + 64.f, j * 128.f + 64.f), vec2(128.f, 128.f), instanceLogicManager->GetTexBK());
		}
	}

	// Render m_entitiesList
	for (unsigned int i = 0; i < instanceLogicManager->m_entitiesList.size(); i++) {
		CORE_RenderCenteredSprite(instanceLogicManager->m_entitiesList[i]->FindComponent<CMP_Transform>()->GetPos(),
			vec2(instanceLogicManager->m_entitiesList[i]->FindComponent<CMP_Collider>()->GetRadius() * 2.f,
				instanceLogicManager->m_entitiesList[i]->FindComponent<CMP_Collider>()->GetRadius() * 2.f),
			instanceLogicManager->m_entitiesList[i]->FindComponent<CMP_Render>()->GetGfxSprite());
	}


	// Text
	int roundElapsed = static_cast<int>(_elapsed);
	std::string s = std::to_string(_elapsed);
	const char* logicCurrentTime = s.c_str();


	int roundTime = static_cast<int>(_currentTime);
	std::string s2 = std::to_string(roundTime);
	const char* realCurrentTime = s2.c_str();


	int roundFps = static_cast<int>(_fps);
	std::string s3 = std::to_string(roundFps);
	const char* currentFPS = s3.c_str();


	FONT_DrawString(vec2(SCR_WIDTH / 2 + 5 * 15, 60), "LOGIC TIME: ");
	FONT_DrawString(vec2(SCR_WIDTH / 2 + 5 * 15, 35), logicCurrentTime);

	FONT_DrawString(vec2(SCR_WIDTH / 2 + 5 * 15, 110), "REAL TIME");
	FONT_DrawString(vec2(SCR_WIDTH / 2 + 5 * 15, 85), realCurrentTime);

	FONT_DrawString(vec2(SCR_WIDTH / 2 + 5 * 15, 160), "CURRENT FPS");
	FONT_DrawString(vec2(SCR_WIDTH / 2 + 5 * 15, 135), currentFPS);

	// Exchanges the front and back buffers
	SYS_Show();
}



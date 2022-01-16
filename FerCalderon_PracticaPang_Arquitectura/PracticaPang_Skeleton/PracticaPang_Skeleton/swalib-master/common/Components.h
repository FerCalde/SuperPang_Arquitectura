#pragma once
#pragma region Manager Includes
#include "EntityAndComponent.h"
#include "MyMessageSystem.h"
#include "MyTimer.h"
#pragma endregion
#include "vector2d.h"
#include "stdafx.h"



class CMP_Transform : public Component
{
private:
	vec2 m_vel;
	vec2 m_pos;
	NewPosMsg* ptrNewPosMsg;

public:
	CMP_Transform();
	virtual ~CMP_Transform();

	void SetPos(const vec2& _pos) { m_pos = _pos; }
	void SetVel(const vec2& _vel) { m_vel = _vel; }

	vec2& GetPos() { return m_pos; }
	vec2& GetVel() { return m_vel; }

	void UpdatePosition(const float& _elapsed) { m_pos += m_vel * _elapsed; }
	
	// Interfaz
	virtual void Slot(const float& _elapsed) override;
	virtual void RecibirMsg(Message* _msgType) override;

};

class CMP_Collider : public Component
{
private:
	CollisionMsg* ptrCollisionMsg;
	LimitWorldCollMsg* ptrLimitCollisionMsg;

public:
	
	float radius = 0;
	CMP_Collider();
	virtual ~CMP_Collider();

	void SetRadius(const float& _radius) { radius = _radius; }
	float& GetRadius() { return radius; }

	vec2& GetPos() { return m_CmpOwner->FindComponent<CMP_Transform>()->GetPos(); }
	vec2& GetVel() { return m_CmpOwner->FindComponent<CMP_Transform>()->GetVel(); }

	bool IsColliding(Entity* _otherEntity);
	
	// Interfaz
	virtual void Slot(const float& _elapsed) override;
	virtual void RecibirMsg(Message* _msgType) override;

	
};

class CMP_Render : public Component
{
private:
	GLuint m_gfx = 0;

public:
	CMP_Render() {}
	virtual ~CMP_Render() {}

	void SetGfxSprite(const GLuint& _gfxSprite);
	GLuint GetGfxSprite() { return m_gfx; }

	// Interfaz
	virtual void Slot(const float& _elapsed) override {};
	virtual void RecibirMsg(Message* _msgType) override {};

};
#pragma once
#include "EntityAndComponent.h"
#include "..\MessageSystem\MyMessageSystem.h"
#include "..\PlayerScripts\PlayerEntity.h"
#include "..\..\Engine\ManagersEngine\InputManager.h"
#include "..\..\Engine\ManagersEngine\LogicManager.h"
#include "..\..\Engine\ManagersEngine\TimerManager.h"
#include "..\..\Engine\miscellaneous\vector2d.h"



class CMP_Transform : public Component
{
private:
	vec2 m_vel;
	vec2 m_pos;
	int m_moveDirection = 0;
	vec2 m_velInit=(10,0);

	NewPosMsg* ptrNewPosMsg;

public:
	CMP_Transform();
	virtual ~CMP_Transform();

	void SetMoveDir(const int& _moveDir) { m_moveDirection = _moveDir; }
	void SetPos(const float& _posX) { m_pos.x = _posX; }
	void SetPos(const vec2& _pos) { m_pos = _pos; }
	void SetVel(const vec2& _vel) { m_vel = _vel; }
	void SetVelInit(const vec2& _velInit) {
		m_velInit = _velInit;
		SetVel(m_velInit);
	}

	int GetMoveDir() { return m_moveDirection; }
	vec2& GetPos() { return m_pos; }
	vec2& GetVel() { return m_vel; }
	vec2& GetVelInit() { return m_velInit; }

	void UpdatePosition(const float& _elapsed) {
		m_pos += m_vel * _elapsed * GetMoveDir();
		/*std::cout << "\nPosition entity `[" << m_CmpOwner->GetID() << "] es->> " << m_CmpOwner->FindComponent<CMP_Transform>()->GetPos().x;*/
	}

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

	float radius = 0.5f;
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

public:
	CMP_Render() : m_symbolObject('8') {  }
	CMP_Render(const char _symbol) :m_symbolObject(_symbol) {  }
	virtual ~CMP_Render() {}

	void SetSymbol(char _objectSymbol);
	char GetSymbol() const;

	// Interfaz
	virtual void Slot(const float& _elapsed) override {};
	virtual void RecibirMsg(Message* _msgType) override {};

private:
	char m_symbolObject;
};

class CMP_InputController :public Component
{
public:
	CMP_InputController() {}
	virtual ~CMP_InputController() {}

	void InputMovement();

	// Interfaz
	virtual void Slot(const float& _elapsed) override;
	virtual void RecibirMsg(Message* _msgType) override {};
};

class CMP_Shooter : public Component
{
public:
	CMP_Shooter() {}
	virtual ~CMP_Shooter() {}


	// Interfaz
	virtual void Slot(const float& _elapsed) override;
	virtual void RecibirMsg(Message* _msgType) override {};


private:
	float m_TimeFireSpawn_MAX = 0.3f;
	float m_TimeFireSpawn = m_TimeFireSpawn_MAX;
public:
	void SpawnBullet(const int& movDir);
};
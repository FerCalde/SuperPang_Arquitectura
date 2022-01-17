#pragma once
#pragma region Manager Includes
#include "EntityAndComponent.h"
#include "MyMessageSystem.h"
#include "MyTimer.h"
#pragma endregion
#include "vector2d.h"
#include "stdafx.h"
#include "InputManager.h"


class CMP_Transform : public Component
{
private:
	vec2 m_vel;
	vec2 m_pos;
	int m_moveDirection = 1;

	NewPosMsg* ptrNewPosMsg;

public:
	CMP_Transform();
	virtual ~CMP_Transform();


	void SetMoveDir(const int& _moveDir) { m_moveDirection = _moveDir; }
	void SetPos(const vec2& _pos) { m_pos = _pos; }
	void SetVel(const vec2& _vel) { m_vel = _vel; }

	int GetMoveDir() { return m_moveDirection; }
	vec2& GetPos() { return m_pos; }
	vec2& GetVel() { return m_vel; }

	void UpdatePosition(const float& _elapsed) { m_pos += m_vel * _elapsed * GetMoveDir(); }

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

class CMP_InputController :public Component
{
private:
	NewMoveDirMsg* ptrNewMoveDirMsg;

public:
	CMP_InputController() {
		ptrNewMoveDirMsg = new NewMoveDirMsg(m_CmpOwner->FindComponent<CMP_Transform>()->GetMoveDir());
	}
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
	float m_TimeFireSpawn_MAX = 1.f;
	//float m_TimeFireSpawn_MAX = 0.3f;
	float m_TimeFireSpawn = 0;
public:
	void SpawnBullet(const int& movDir);
};


class CMP_DamageMaker : public Component
{
public:
	CMP_DamageMaker(const int& _hitDamage = 1);
	
	virtual ~CMP_DamageMaker() 
	{
		delete ptrNewDamageMakeMsg;
		ptrNewDamageMakeMsg = nullptr;
	}


	// Interfaz
	virtual void Slot(const float& _elapsed) override {}
	virtual void RecibirMsg(Message* _msgType) override;


private:
	int m_hitDamage = 1;
	DamageMakeMsg* ptrNewDamageMakeMsg;

public :
	void MakeDamage(Entity* _otherEntity);
	

	int GetHitDamage(){ return m_hitDamage; }
	void SetHitDamage(const int& _hitDamage = 1) { m_hitDamage = _hitDamage; }
};



class CMP_LifeBase : public Component
{
public:
	CMP_LifeBase() { OnActivateGO(); }
	virtual ~CMP_LifeBase() {}


	// Interfaz
	virtual void Slot(const float& _elapsed) override;
	virtual void RecibirMsg(Message* _msgType) override;



private:
	int m_lifeMax = 3;
	int m_currentLife = m_lifeMax;
public:
	int GetLife() { return m_currentLife; }
	void SetLife(const int& _life);

	void OnActivateGO(); //Recibe mensaje cuando se activa un GO (Por hacer Spawn) y rellena la vida al maximo

	void TakeDamage(const int& _damage = 1); //Por defecto quita 1, se puede modificar si se quiere un juego con vida en base a 100 o quitar dos vidas (algun power up)por ejemplo.
	
	//Interfaz de la vida
	virtual void IsDead() = 0;
};

class CMP_LifePlayer :public CMP_LifeBase
{
public :
	CMP_LifePlayer() { }
	virtual ~CMP_LifePlayer() {}


	// Interfaz
	virtual void Slot(const float& _elapsed) override {  }
	virtual void RecibirMsg(Message* _msgType) override { CMP_LifeBase::RecibirMsg(_msgType); }

	//Propia de Life
	virtual void IsDead() { m_CmpOwner->DesactivateEntity(); }

};

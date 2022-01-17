#pragma once
#include "sys.h"
#include "vector2d.h"
#include "EntityAndComponent.h"

class Message
{
public:
	Message() {}
	virtual ~Message() {}
};

#pragma region TransformMsg
class NewPosMsg : public Message
{
private:
	vec2 m_pos;

public:
	NewPosMsg() { }
	NewPosMsg(vec2& _NewPosition) : m_pos(_NewPosition) { }
	virtual ~NewPosMsg() { }

	void SetNewPos(vec2 _Position) { m_pos = _Position; }
	vec2 GetNewPos() { return m_pos;}
};

class NewVelMsg : public Message
{
private:
	vec2 m_vel;

public:

	NewVelMsg() { }
	NewVelMsg(vec2& _NewVelocity) : m_vel(_NewVelocity) { }
	virtual ~NewVelMsg() { }

	void SetNewVel(vec2 _NewVelocity) { m_vel = _NewVelocity; }
	vec2 GetNewVel() { return m_vel; }

};

class NewMoveDirMsg : public Message
{
private :
	int m_moveDir;

public:
	//NewMoveDirMsg(){}
	NewMoveDirMsg(int _NewMoveDir): m_moveDir(_NewMoveDir){}
	virtual ~NewMoveDirMsg() { }

	void SetNewMoveDir(const int& _NewMoveDir) { m_moveDir = _NewMoveDir; }
	int GetNewMoveDir() { return m_moveDir; }
};
#pragma endregion

#pragma region CollisionMsg
class CollisionMsg : public Message
{
private:
		bool isColliding;
		int entityListIndex;
		bool canDamage = false;
public:
		CollisionMsg() : isColliding(false), entityListIndex(-1) { }
		CollisionMsg(bool _isColliding, int _entityListIndex) : isColliding(_isColliding), entityListIndex(_entityListIndex) {}
		virtual ~CollisionMsg() { }

		void SetCollision(bool _isColliding) { isColliding = _isColliding; }
		void SetIndex(int _entityListIndex) { entityListIndex = _entityListIndex; }
		void SetCanDamage(bool _canDamage) { canDamage = _canDamage; }

		bool GetCollision() { return isColliding; }
		int GetIndex() { return entityListIndex; }
		bool GetCanDamage(){ return canDamage; }
};

class EntCollisionMsg : public CollisionMsg
{
private:
	float invertSpeed;

public:
	EntCollisionMsg() : invertSpeed(-1.f) { }
	virtual ~EntCollisionMsg() { }

	// SETTER
	void SetInvertChangeSpeed(float _Rebound) { invertSpeed = _Rebound; }

	// GETTER
	float GetInvertChangeSpeed() { return invertSpeed; }
};

class LimitWorldCollMsg : public CollisionMsg
{
private:
	const int limit_SCR_WIDTH;
	const int limit_SCR_HEIGHT;

public:
	LimitWorldCollMsg() : limit_SCR_WIDTH(SCR_WIDTH), limit_SCR_HEIGHT(SCR_HEIGHT) { }
	virtual ~LimitWorldCollMsg() { }

	const int GetLimitWidth() const { return limit_SCR_WIDTH; }
	const int GetLimitHeight() const { return limit_SCR_HEIGHT; }
};
#pragma endregion


class DamageMakeMsg : public Message
{
	
public:
	DamageMakeMsg(const int& _damage = 1) { SetDamage(_damage); }
	void SetDamage(const int& _damage) { m_damage = _damage; }
	int GetDamage() { return m_damage; }

private:
	int m_damage = 1; //Minimun MUST be 1;

};


class OnActiveEntityMsg : public Message
{

public:
	OnActiveEntityMsg(){}
	

};

class FireMsg : public Message
{

public:
	FireMsg(){}

};

class ActiveStateCMPMsg : public Message //Mensaje que se envia para activar/desactivarCMP
{
	ActiveStateCMPMsg(){}

};
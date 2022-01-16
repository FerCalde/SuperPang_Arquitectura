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
#pragma endregion

#pragma region CollisionMsg
class CollisionMsg : public Message
{
private:
		bool isColliding;
		int entityListIndex;

public:
		CollisionMsg() : isColliding(false), entityListIndex(-1) { }
		CollisionMsg(bool _isColliding, int _entityListIndex) : isColliding(_isColliding), entityListIndex(_entityListIndex) {}
		virtual ~CollisionMsg() { }

		void SetCollision(bool _isColliding) { isColliding = _isColliding; }
		void SetIndex(int _entityListIndex) { entityListIndex = _entityListIndex; }

		bool GetCollision() { return isColliding; }
		int GetIndex() { return entityListIndex; }
};

class EntCollisionMsg : public CollisionMsg
{
private:
	float reboteChangeSpeed;

public:
	EntCollisionMsg() : reboteChangeSpeed(-1.f) { }
	virtual ~EntCollisionMsg() { }

	// SETTER
	void SetReboteChangeSpeed(float _Rebound) { reboteChangeSpeed = _Rebound; }

	// GETTER
	float GetReboteChangeSpeed() { return reboteChangeSpeed; }
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
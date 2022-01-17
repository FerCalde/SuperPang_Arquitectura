#pragma once

#include "Components.h"
#include "stdafx.h"
#include "vector2d.h"
#include "sys.h"


//ALL ENTITIES WILL BE GAME OBJECTS AND MUUUUUUST HAVE A CMP_TRANSFORM
class GameObject : public Entity
{

public:
	GameObject()
	{
		SetTag(ETagEntity::Invalid);
		AddComponent(new CMP_Transform);
	}

	virtual ~GameObject() {};

};



class Ball : public GameObject
{

public:
	Ball()
	{
		SetTag(ETagEntity::Enemy);
		SetTagIgnore(ETagEntity::Invalid);
		
		AddComponent(new CMP_Collider);
		AddComponent(new CMP_DamageMaker);
		AddComponent(new CMP_Render);
		AddComponent(new CMP_LifeEnemy);
	}

	virtual ~Ball() {};

};




class GameObject;
class Player :public GameObject
{
public:
	Player()
	{
		SetTag(ETagEntity::Player);
		SetTagIgnore(ETagEntity::Bullet);
		
		AddComponent(new CMP_InputController);
		AddComponent(new CMP_Collider);
		AddComponent(new CMP_DamageMaker);
		//AddComponent(new CMP_LifePlayer);
		AddComponent(new CMP_Render);
		AddComponent(new CMP_SpawnerEntity);

	}

	virtual ~Player() {};

};

class GameObject;
class Bullet : public GameObject
{
public:
	Bullet()
	{
		SetTag(ETagEntity::Bullet);
		SetTagIgnore(ETagEntity::Player);

		AddComponent(new CMP_Collider);
		AddComponent(new CMP_DamageMaker);
		AddComponent(new CMP_LifeBullet);
		AddComponent(new CMP_Render);

	}
};


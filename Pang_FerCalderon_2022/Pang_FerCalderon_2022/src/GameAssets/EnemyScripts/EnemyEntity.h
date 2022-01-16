#pragma once
#include"..\EntityComponentSystem\Components.h"
#include "..\EntityComponentSystem\EntityAndComponent.h"

class CEnemy : public Entity
{
public:
	CEnemy()
	{
		SetTag(Entity::ETagEntity::Enemy);
		m_eTagIgnore = Entity::ETagEntity::Enemy;
		
		/*Component* auxT = new CMP_Transform;
		AddComponent(auxT);

		auxT = new CMP_Collider;
		AddComponent(auxT);

		auxT = new CMP_Render;
		AddComponent(auxT);


		auxT = nullptr;*/
	}
	~CEnemy(){}
};
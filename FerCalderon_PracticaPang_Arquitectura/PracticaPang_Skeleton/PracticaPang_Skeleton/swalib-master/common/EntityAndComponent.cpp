#include "EntityAndComponent.h"

Entity::~Entity()
{
	for (auto& cmp : m_ComponentList)
	{
		delete cmp;
		cmp = nullptr;
	}
	m_ComponentList.clear();
}

void Entity::Slot(float _elapsed)
{
	if (this->IsActive())
	{
		for (auto& cmp : m_ComponentList)
		{
			cmp->Slot(_elapsed);
		}
	}
}

void Entity::AddComponent(Component* _componentAdded)
{
	_componentAdded->m_CmpOwner = this;
	m_ComponentList.push_back(_componentAdded);
}

void Entity::SendMsg(Message* _msgType)
{
	for (auto& cmp : m_ComponentList)
	{
		cmp->RecibirMsg(_msgType);
	}
}
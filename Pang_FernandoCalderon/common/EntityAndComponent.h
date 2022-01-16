#pragma once
#include <iostream>
#include <vector>
#include "MyMessageSystem.h"

class Entity;
class Message;

class Component
{
protected: 
	Entity* m_CmpOwner;

private:
	friend class Entity;

public:
	Component() { m_CmpOwner = nullptr; }
	virtual ~Component() { }

	virtual void Slot(const float& _elapsed) = 0;
	virtual void RecibirMsg(Message* _msgType) = 0;

};

class Entity
{
private:
	size_t id = 0;
	std::vector<Component*> m_ComponentList;

public:
	Entity() { }
	~Entity();

	void SetID(size_t _ID) { id = _ID; }
	size_t GetID() { return id; }

	void Slot(float _elapsed);
	void AddComponent(Component* _componentAdded);
	void SendMsg(Message* _msgType);

	template<typename T>
	T* FindComponent()
	{
		for (auto& cmp : m_ComponentList)
		{
			T* component = dynamic_cast<T*>(cmp);
			if (component)
				return component;
		}
		return NULL;
	}
};
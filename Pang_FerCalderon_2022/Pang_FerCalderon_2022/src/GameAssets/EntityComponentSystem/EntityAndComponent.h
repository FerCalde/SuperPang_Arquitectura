#pragma once
#include <iostream>
#include <vector>
#include "..\MessageSystem\MyMessageSystem.h"

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
	Entity() { SetTag(ETagEntity::Invalid); }
	~Entity();

	void SetID(size_t _ID) { id = _ID; }
	size_t GetID() { return id; }

	void Slot(float _elapsed);
	void AddComponent(Component* _componentAdded);
	void SendMsg(Message* _msgType);

	void SetActive(bool _isActive) { isActive = _isActive; }
	void ActivateEntity() {
		/*vec2 auxVelInit = FindComponent<CMP_Transform>()->GetVelInit();
		FindComponent<CMP_Transform>()->SetVelInit(auxVelInit);*/

		isActive = true;
	}
	void DesactivateEntity() { isActive = false; }
	bool IsActive() { return isActive; }

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

protected:
	bool isActive = false;
public:
	enum class ETagEntity
	{
		Invalid = -1,
		Player, Enemy, Bullet
	};

	ETagEntity GetTag() const { return m_eTag; }
	bool HasTag(ETagEntity _eTag) const { return m_eTag == _eTag; }
	void SetTag(const ETagEntity& _eTag) { m_eTag = _eTag; }
	ETagEntity m_eTagIgnore;
private:
	ETagEntity m_eTag;
};
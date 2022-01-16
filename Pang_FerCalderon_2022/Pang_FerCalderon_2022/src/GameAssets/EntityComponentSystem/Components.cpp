#include "Components.h"

#pragma region CMP_Collider

CMP_Collider::CMP_Collider()
{
	ptrCollisionMsg = new CollisionMsg(false, -1);
	ptrLimitCollisionMsg = new LimitWorldCollMsg();
}

CMP_Collider::~CMP_Collider()
{
	delete ptrCollisionMsg;
	ptrCollisionMsg = nullptr;

	delete ptrLimitCollisionMsg;
	ptrLimitCollisionMsg = nullptr;
}

bool CMP_Collider::IsColliding(Entity* _otherEntity)
{
	if (this->m_CmpOwner != _otherEntity)
	{
		if (m_CmpOwner->IsActive())
		{
			if (_otherEntity->IsActive())
			{

				if ((m_CmpOwner->GetTag() != _otherEntity->GetTag()) && (m_CmpOwner->m_eTagIgnore != _otherEntity->GetTag()))
				{

					CMP_Transform* pMyTransform = m_CmpOwner->FindComponent<CMP_Transform>();
					CMP_Transform* pOtherTransform = _otherEntity->FindComponent<CMP_Transform>();
					int iMyDirection = m_CmpOwner->FindComponent<CMP_Transform>()->GetMoveDir();

					if (((pMyTransform->GetPos().x - pOtherTransform->GetPos().x) <= 1.f) &&
						((pMyTransform->GetPos().x - pOtherTransform->GetPos().x) >= -1.f))
					{
						if ((((pMyTransform->GetPos().x + iMyDirection) - pOtherTransform->GetPos().x) <= 1.f) &&
							(((pMyTransform->GetPos().x + iMyDirection) - pOtherTransform->GetPos().x) >= -0.5f))
						{
							return true;
						}
					}

				}
			}
		}
	}

	return false;


}

void CMP_Collider::Slot(const float& _elapsed)
{
	for (auto& _otherEntity : LogicManager::GetInstance().m_entitiesList)
	{
		if (IsColliding(_otherEntity))
		{
			ptrCollisionMsg->SetCollision(true);
			ptrCollisionMsg->SetIndex(_otherEntity->GetID());
			m_CmpOwner->SendMsg(ptrCollisionMsg);

			//Avisar al otro entity de la colision
			ptrCollisionMsg->SetIndex(m_CmpOwner->GetID());
			_otherEntity->SendMsg(ptrCollisionMsg);
			break;
		}

	}
	// Player y Bullets comprueban si se han salido del mapa
	if ( m_CmpOwner->HasTag(Entity::ETagEntity::Bullet))
	{
		m_CmpOwner->SendMsg(ptrLimitCollisionMsg);
	}
}

void CMP_Collider::RecibirMsg(Message* _msgType)
{
	CollisionMsg* collisionMsg = dynamic_cast<CollisionMsg*>(_msgType);
	if (collisionMsg)
	{
		if (collisionMsg->GetCollision())
		{
			//Resolver Colisiones Individuales
			if (m_CmpOwner->HasTag(Entity::ETagEntity::Enemy))
			{
				LogicManager::GetInstance().m_GameManager->AddEnemyDead();
			}
			if (m_CmpOwner->HasTag(Entity::ETagEntity::Player))
			{
				LogicManager::GetInstance().m_GameManager->UpdatePlayerLife(false);
			}
			//En cualquier caso, desactivar el Entity.
			m_CmpOwner->DesactivateEntity();
		}
	}

	LimitWorldCollMsg* auxLimitCollMsg = dynamic_cast<LimitWorldCollMsg*>(_msgType);
	if (auxLimitCollMsg)
	{
		if ((GetPos().x > RenderEngine::GetInstance().WEIGHT_MAP-1.f) || (GetPos().x < 0))
		{
			m_CmpOwner->FindComponent<CMP_Transform>()->SetMoveDir(0);
				m_CmpOwner->DesactivateEntity();
		}
		
	}
}
#pragma endregion

#pragma region CMP_InputController


void CMP_InputController::Slot(const float& _elapsed)
{
	InputMovement();
}

void CMP_InputController::InputMovement()
{
	if (CInputManager::GetInstance().IsKeyPressed(KEYBOARD_A))
	{
		if (m_CmpOwner->FindComponent<CMP_Transform>()->GetPos().x > 0)
		{
			m_CmpOwner->FindComponent<CMP_Transform>()->SetMoveDir(-1);
		}
		else 
		{
			m_CmpOwner->FindComponent<CMP_Transform>()->SetMoveDir(0);

		}
	}
	else if (CInputManager::GetInstance().IsKeyPressed(KEYBOARD_D))
	{
		if (m_CmpOwner->FindComponent<CMP_Transform>()->GetPos().x < RenderEngine::GetInstance().WEIGHT_MAP-3.f)
		{
			m_CmpOwner->FindComponent<CMP_Transform>()->SetMoveDir(1);
		}
		else
		{
			m_CmpOwner->FindComponent<CMP_Transform>()->SetMoveDir(0);

		}
	}
	else
	{
		m_CmpOwner->FindComponent<CMP_Transform>()->SetMoveDir(0);
	}
}
#pragma endregion

#pragma region CMP_Render

void CMP_Render::SetSymbol(char _objectSymbol)
{
	m_symbolObject = _objectSymbol;
}

char CMP_Render::GetSymbol() const
{
	return m_symbolObject;
}
#pragma endregion

#pragma region CMP_Shooter

void CMP_Shooter::Slot(const float& _elapsed)
{
	// Chequear si ha pasado el tiempo entre disparos
	if (m_TimeFireSpawn >= m_TimeFireSpawn_MAX)//)m_fTimeUntilNextAttack == 0.f)
	{
		// Comprobar si se ha pulsado alguna tecla asignada al disparo
		int auxFireDirection = 0;
		if (CInputManager::GetInstance().IsKeyPressed(KEYBOARD_H))
		{
			auxFireDirection = -1;
			//std::cout << "Left FIRE!\n";
		}
		else if (CInputManager::GetInstance().IsKeyPressed(KEYBOARD_K))
		{
			auxFireDirection = 1;
			//std::cout << "RIGHT FIRE!\n";
		}
		// En caso de detectar input, spawnear bala
		if (auxFireDirection != 0)
		{
			SpawnBullet(auxFireDirection);
			// Restablecer fireRate
			m_TimeFireSpawn = 0;
		}
	}
	else
	{
		//Temporizador FireRate
		m_TimeFireSpawn += _elapsed;
		if (m_TimeFireSpawn > m_TimeFireSpawn_MAX)
		{
			m_TimeFireSpawn = m_TimeFireSpawn_MAX;
		}
	}


}

void CMP_Shooter::SpawnBullet(const int& movDir)
{

	for (Entity* currentEntity : LogicManager::GetInstance().m_entitiesList)
	{
		if (!currentEntity->IsActive())
		{
			if (currentEntity->HasTag(Entity::ETagEntity::Bullet))
			{
				//Pido una bala disponible al Gestor de entidades. Debe estar desactivada esa bala para considerarse disponible
				std::cout << "FIRE!\n";

				vec2 auxFirePoint(0, 0);

				auxFirePoint.x = m_CmpOwner->FindComponent<CMP_Transform>()->GetPos().x + movDir;
				currentEntity->FindComponent<CMP_Transform>()->SetPos(auxFirePoint);

				vec2 auxVelInit(m_CmpOwner->FindComponent<CMP_Transform>()->GetVelInit());
				currentEntity->FindComponent<CMP_Transform>()->SetVel(auxVelInit);

				currentEntity->FindComponent<CMP_Transform>()->SetMoveDir(movDir);
				(movDir > 0) ? currentEntity->FindComponent<CMP_Render>()->SetSymbol('>') : currentEntity->FindComponent<CMP_Render>()->SetSymbol('<');
				currentEntity->ActivateEntity();
				break;
			}
		}
	}
	//auxBullet = nullptr;
}
#pragma endregion

#pragma region CMP_Transform

CMP_Transform::CMP_Transform()
{
	ptrNewPosMsg = new NewPosMsg(GetPos());
	SetVelInit(10);
}

CMP_Transform::~CMP_Transform()
{
	delete ptrNewPosMsg;
	ptrNewPosMsg = nullptr;
}

void CMP_Transform::Slot(const float& _elapsed)
{
	UpdatePosition(_elapsed);
	ptrNewPosMsg->SetNewPos(GetPos());
	m_CmpOwner->SendMsg(ptrNewPosMsg);
}

void CMP_Transform::RecibirMsg(Message* _msgType)
{
	NewPosMsg* auxPosMsg = dynamic_cast<NewPosMsg*>(_msgType);
	if (auxPosMsg)
	{
		SetPos(auxPosMsg->GetNewPos());
	}

	NewVelMsg* auxVelMsg = dynamic_cast<NewVelMsg*>(_msgType);
	if (auxVelMsg)
	{
		SetVel(auxVelMsg->GetNewVel());
	}
}
#pragma endregion








#include "Components.h"

#include "InputManager.h"
#include "LogicManager.h"
#include "core.h"
#include "sys.h"

#pragma region CMP_Transform
CMP_Transform::CMP_Transform()
{
	ptrNewPosMsg = new NewPosMsg(GetPos());
}

CMP_Transform::~CMP_Transform()
{
	delete ptrNewPosMsg;
	ptrNewPosMsg = nullptr;
}


void CMP_Transform::Slot(const float& _elapsed)
{
	if (IsActive())
	{
		UpdatePosition(_elapsed);
		ptrNewPosMsg->SetNewPos(GetPos());
		m_CmpOwner->SendMsg(ptrNewPosMsg);
	}
}

void CMP_Transform::RecibirMsg(Message* _msgType)
{
	if (IsActive())
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

		NewMoveDirMsg* auxMoveDirMsg = dynamic_cast<NewMoveDirMsg*>(_msgType);
		if (auxMoveDirMsg)
		{
			SetMoveDir(auxMoveDirMsg->GetNewMoveDir());
		}
	}


}
#pragma endregion

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
	LogicManager* instanceLogicManager = LogicManager::GetInstance();


	if (_otherEntity->IsActive() && _otherEntity->FindComponent<CMP_Collider>())
	{

		float dist = (GetRadius() + instanceLogicManager->m_entitiesList[_otherEntity->GetID()]->FindComponent<CMP_Collider>()->GetRadius())
			* (GetRadius() + instanceLogicManager->m_entitiesList[_otherEntity->GetID()]->FindComponent<CMP_Collider>()->GetRadius());

		if (this->m_CmpOwner != _otherEntity)
		{
			if (!_otherEntity->HasTag(m_CmpOwner->GetIgnoreTag()))
			{

				if (vlen2(GetPos() - instanceLogicManager->m_entitiesList[_otherEntity->GetID()]->FindComponent<CMP_Transform>()->GetPos()) <= dist)
				{
					return true;
				}
			}
		}
	}
	return false;


}

void CMP_Collider::Slot(const float& _elapsed)
{
	if (IsActive())
	{
		for (auto& _otherEntity : LogicManager::GetInstance()->m_entitiesList)
		{
			if (IsColliding(_otherEntity))
			{
				ptrCollisionMsg->SetCollision(true);
				ptrCollisionMsg->SetIndex(_otherEntity->GetID());

				//HACER DAÑO A LA ENTIDAD
				//Entidades con mismo tag no deben hacerse damage entre si. 
				_otherEntity->HasTag(m_CmpOwner->GetTag()) ? ptrCollisionMsg->SetCanDamage(false) : ptrCollisionMsg->SetCanDamage(true);;


				m_CmpOwner->SendMsg(ptrCollisionMsg);


				break;
			}
		}

		m_CmpOwner->SendMsg(ptrLimitCollisionMsg);
	}
}

void CMP_Collider::RecibirMsg(Message* _msgType)
{
	if (IsActive())
	{


		CollisionMsg* collisionMsg = dynamic_cast<CollisionMsg*>(_msgType);
		if (collisionMsg)
		{
			if (collisionMsg->GetCollision())
			{

				EntCollisionMsg* auxEntColMsg = new EntCollisionMsg();
				m_CmpOwner->SendMsg(auxEntColMsg);

				//Optimizacion
				delete auxEntColMsg;
				auxEntColMsg = nullptr;
			}
			//else
			//{
			//	//NewPosMsg* auxPosMsg = new NewPosMsg(GetPos());
			//	//m_CmpOwner->SendMsg(auxPosMsg);

			//	////Optimizacion
			//	//delete auxPosMsg;
			//	//auxPosMsg = nullptr;
			//}
		}

		EntCollisionMsg* auxEntColMsg = dynamic_cast<EntCollisionMsg*>(_msgType);
		if (auxEntColMsg)
		{
			if (m_CmpOwner->HasTag(Entity::ETagEntity::Enemy))
			{
				NewVelMsg* auxVelMsg = new NewVelMsg(GetVel() * auxEntColMsg->GetInvertChangeSpeed());
				m_CmpOwner->SendMsg(auxVelMsg);

				//Optimizacion
				delete auxVelMsg;
				auxVelMsg = nullptr;

			}
		}

		LimitWorldCollMsg* auxLimitCollMsg = dynamic_cast<LimitWorldCollMsg*>(_msgType);
		if (auxLimitCollMsg)
		{
			if ((GetPos().x > auxLimitCollMsg->GetLimitWidth()) || (GetPos().x < 0))
			{
				NewVelMsg* auxVelMsg = new NewVelMsg(vec2(GetVel().x * -1.0f, GetVel().y));
				m_CmpOwner->SendMsg(auxVelMsg);

				//Optimizacion
				delete auxVelMsg;
				auxVelMsg = nullptr;
			}
			if ((GetPos().y > auxLimitCollMsg->GetLimitHeight()) || (GetPos().y < 0))
			{
				if (m_CmpOwner->HasTag(Entity::ETagEntity::Enemy))
				{

					NewVelMsg* auxVelMsg = new NewVelMsg(vec2(GetVel().x, GetVel().y * -1.0f));
					m_CmpOwner->SendMsg(auxVelMsg);

					//Optimizacion
					delete auxVelMsg;
					auxVelMsg = nullptr;
				}
				if (m_CmpOwner->HasTag(Entity::ETagEntity::Bullet))
				{
					DamageMakeMsg* auxDamageMsg = new DamageMakeMsg();
					m_CmpOwner->SendMsg(auxDamageMsg);

					delete auxDamageMsg;
					auxDamageMsg = nullptr;
				}

			}
		}
	}
}
#pragma endregion

#pragma region CMP_Render

void CMP_Render::SetGfxSprite(const GLuint& _gfxSprite)
{
	m_gfx = _gfxSprite;
}

#pragma endregion

#pragma region CMP_InputController


void CMP_InputController::Slot(const float& _elapsed)
{
	InputMovement();
	InputFire();
}

CMP_InputController::CMP_InputController()
{
	ptrNewMoveDirMsg = new NewMoveDirMsg(0);
	ptrFireMsg = new FireMsg();
}

CMP_InputController::~CMP_InputController()
{
	delete ptrNewMoveDirMsg;
	ptrNewMoveDirMsg = nullptr;

	delete ptrFireMsg;
	ptrFireMsg = nullptr;
}

void CMP_InputController::InputMovement()
{
	if (CInputManager::GetInstance().PressedMoveLeft())
	{
		if (m_CmpOwner->FindComponent<CMP_Transform>()->GetPos().x > (0 + 5))
		{
			ptrNewMoveDirMsg->SetNewMoveDir(-1);
		}
		else
		{
			ptrNewMoveDirMsg->SetNewMoveDir(0);
		}
	}
	else if (CInputManager::GetInstance().PressedMoveRight())
	{
		if (m_CmpOwner->FindComponent<CMP_Transform>()->GetPos().x < (SCR_WIDTH - 5))
		{
			ptrNewMoveDirMsg->SetNewMoveDir(1);

		}
		else
		{
			ptrNewMoveDirMsg->SetNewMoveDir(0);
		}
	}
	else
	{
		ptrNewMoveDirMsg->SetNewMoveDir(0);
	}

	m_CmpOwner->SendMsg(ptrNewMoveDirMsg);

}

void CMP_InputController::InputFire()
{
	if (CInputManager::GetInstance().PressedFire())
	{
		m_CmpOwner->SendMsg(ptrFireMsg);
	}

}


#pragma endregion

#pragma region CMP_SpawnerEntity

void CMP_SpawnerEntity::Slot(const float& _elapsed)
{
	// Chequear si ha pasado el tiempo entre disparos
	if (m_TimeFireSpawn >= m_TimeFireSpawn_MAX)//)m_fTimeUntilNextAttack == 0.f)
	{

		/*for (auto& entityNotEnabled : LogicManager::GetInstance()->m_entitiesList)
		{
			if (entityNotEnabled->HasTag(Entity::ETagEntity::Enemy))
			{

				if (entityNotEnabled->IsActive())
				{
					entityNotEnabled->DesactivateEntity();
				}
				else if (!entityNotEnabled->IsActive())
				{
					entityNotEnabled->ActivateEntity();
				}



			}

		}*/


		// Restablecer fireRate
		m_TimeFireSpawn = 0;


		//// Comprobar si se ha pulsado alguna tecla asignada al disparo
		//int auxFireDirection = 0;
		//if (CInputManager::GetInstance().IsKeyPressed(KEYBOARD_H))
		//{
		//	auxFireDirection = -1;
		//	//std::cout << "Left FIRE!\n";
		//}
		//else if (CInputManager::GetInstance().IsKeyPressed(KEYBOARD_K))
		//{
		//	auxFireDirection = 1;
		//	//std::cout << "RIGHT FIRE!\n";
		//}
		//// En caso de detectar input, spawnear bala
		//if (auxFireDirection != 0)
		//{
		//	SpawnBullet(auxFireDirection);
		//	// Restablecer fireRate
		//	m_TimeFireSpawn = 0;
		//}
	}
	else
	{

		//Temporizador FireRate
		m_TimeFireSpawn += _elapsed;
		if (m_TimeFireSpawn > m_TimeFireSpawn_MAX)
		{
			canSpawn = true;
			m_TimeFireSpawn = m_TimeFireSpawn_MAX;
		}
	}


}

void CMP_SpawnerEntity::RecibirMsg(Message* _msgType)
{
	/*COMPORTAMIENTO PARA DISPARO DEL PLAYER*/
	if (canSpawn)
	{

		FireMsg* auxFireMsg = dynamic_cast<FireMsg*>(_msgType);
		if (auxFireMsg)
		{
			SpawnEntity(Entity::ETagEntity::Bullet);
			canSpawn = false;
			// Restablecer fireRate
			m_TimeFireSpawn = 0;
		}

		auxFireMsg = nullptr;
	}
	/*COMPORTAMIENTO PARA DISPARO DEL PLAYER*/
}

void CMP_SpawnerEntity::SpawnBullet(const int& movDir)
{
	for (Entity* currentEntity : LogicManager::GetInstance()->m_entitiesList)
	{
		if (!currentEntity->IsActive())
		{
			if (currentEntity->HasTag(Entity::ETagEntity::Bullet))
			{
				//Pido una bala disponible al Gestor de entidades. Debe estar desactivada esa bala para considerarse disponible
				std::cout << "FIRE!\n";

				vec2 auxFirePoint(currentEntity->FindComponent<CMP_Transform>()->GetPos().x, currentEntity->FindComponent<CMP_Transform>()->GetPos().y + 15.f /*Offset superior*/);

				auxFirePoint.y = m_CmpOwner->FindComponent<CMP_Transform>()->GetPos().y + movDir;

				NewPosMsg* auxPosMsg = new NewPosMsg(auxFirePoint);
				currentEntity->SendMsg(auxPosMsg);


				////currentEntity->FindComponent<CMP_Transform>()->SetPos(auxFirePoint);

				/*vec2 auxVelInit(m_CmpOwner->FindComponent<CMP_Transform>()->GetVelInit());
				currentEntity->FindComponent<CMP_Transform>()->SetVel(auxVelInit);

				currentEntity->FindComponent<CMP_Transform>()->SetMoveDir(movDir);
				(movDir > 0) ? currentEntity->FindComponent<CMP_Render>()->SetSymbol('>') : currentEntity->FindComponent<CMP_Render>()->SetSymbol('<');*/
				currentEntity->ActivateEntity();
				break;
			}
		}
	}
	//auxBullet = nullptr;
}

void CMP_SpawnerEntity::SpawnEntity(Entity::ETagEntity _entitySpawnTag)
{

	for (Entity* currentEntity : LogicManager::GetInstance()->m_entitiesList)
	{
		if (!currentEntity->IsActive())
		{
			if (currentEntity->HasTag(_entitySpawnTag))
			{
				//Pido un Entity disponible al Gestor de entidades. Debe estar desactivada esa bala para considerarse disponible
				std::cout << "SPAWN!\n";

				//ptrPosSpawn = new vec2(m_CmpOwner->FindComponent<CMP_Transform>()->GetPos());
				//Mensaje para setear posicion y velocidad del Objeto Spawneado

				ptrNewPosActiveMsg->SetNewPos(m_CmpOwner->FindComponent<CMP_Transform>()->GetPos());
				currentEntity->SendMsg(ptrNewPosActiveMsg);

				//Mensaje para Activar el GO
				currentEntity->SendMsg(onActiveEntMsg);

				currentEntity->ActivateEntity();
				break;
				/*delete ptrPosSpawn;
				ptrPosSpawn = nullptr;*/



				/*for (auto& entityNotEnabled : LogicManager::GetInstance()->m_entitiesList)
				{
					if (entityNotEnabled->HasTag(Entity::ETagEntity::Enemy))
					{
						if (entityNotEnabled->IsActive())
						{
							entityNotEnabled->DesactivateEntity();
						}
						else if (!entityNotEnabled->IsActive())
						{
							entityNotEnabled->ActivateEntity();
						}



					}

				}*/
			}

		}

	}
}


//void CMP_SpawnerEntity::SpawnBullet(const int& movDir)
//{
//	for (Entity* currentEntity : LogicManager::GetInstance()->m_entitiesList)
//	{
//		if (!currentEntity->IsActive())
//		{
//			if (currentEntity->HasTag(Entity::ETagEntity::Bullet))
//			{
//				//Pido una bala disponible al Gestor de entidades. Debe estar desactivada esa bala para considerarse disponible
//				std::cout << "FIRE!\n";
//
//				vec2 auxFirePoint(currentEntity->FindComponent<CMP_Transform>()->GetPos().x, 0);
//
//				auxFirePoint.y = m_CmpOwner->FindComponent<CMP_Transform>()->GetPos().y + movDir;
//
//				NewPosMsg* auxPosMsg = new NewPosMsg(auxFirePoint);
//				currentEntity->SendMsg(auxPosMsg);
//
//
//				////currentEntity->FindComponent<CMP_Transform>()->SetPos(auxFirePoint);
//
//				/*vec2 auxVelInit(m_CmpOwner->FindComponent<CMP_Transform>()->GetVelInit());
//				currentEntity->FindComponent<CMP_Transform>()->SetVel(auxVelInit);
//
//				currentEntity->FindComponent<CMP_Transform>()->SetMoveDir(movDir);
//				(movDir > 0) ? currentEntity->FindComponent<CMP_Render>()->SetSymbol('>') : currentEntity->FindComponent<CMP_Render>()->SetSymbol('<');*/
//				currentEntity->ActivateEntity();
//				break;
//			}
//		}
//	}
//	//auxBullet = nullptr;
//}
#pragma endregion

#pragma region CMP_LifeBase
void CMP_LifeBase::Slot(const float& _elapsed)
{
	if (IsActive())
	{

	}
}

void CMP_LifeBase::RecibirMsg(Message* _msgType)
{
	if (IsActive())
	{
		DamageMakeMsg* auxDamageMsg = dynamic_cast<DamageMakeMsg*>(_msgType);
		if (auxDamageMsg)
		{
			TakeDamage(auxDamageMsg->GetDamage());
		}

		auxDamageMsg = nullptr;
	}

	OnActiveEntityMsg* auxOnActiveEntMsg = dynamic_cast<OnActiveEntityMsg*>(_msgType);
	if (auxOnActiveEntMsg)
	{
		RefillLifes();
	}

}

void CMP_LifeBase::SetLife(const int& _life)
{
	m_currentLife = _life;
}

void CMP_LifeBase::RefillLifes()
{
	m_currentLife = m_lifeMax;
}

void CMP_LifeBase::TakeDamage(const int& _damage)
{
	m_currentLife -= _damage;
	if (m_currentLife <= 0)
	{
		IsDead();
	}
}
#pragma endregion

#pragma region CMP_DamageMaker

CMP_DamageMaker::CMP_DamageMaker(const int& _hitDamage)
{
	SetHitDamage(_hitDamage);
	ptrNewDamageMakeMsg = new DamageMakeMsg();
	ptrNewDamageMakeMsg->SetDamage(GetHitDamage());
}

void CMP_DamageMaker::RecibirMsg(Message* _msgType)
{
	CollisionMsg* collisionMsg = dynamic_cast<CollisionMsg*>(_msgType);
	if (collisionMsg)
	{
		if (collisionMsg->GetCollision())
		{
			if (collisionMsg->GetCanDamage())
			{
				MakeDamage(LogicManager::GetInstance()->m_entitiesList[collisionMsg->GetIndex()]);
			}
		}

	}
}

void CMP_DamageMaker::MakeDamage(Entity* _otherEntity)
{

	ptrNewDamageMakeMsg->SetDamage(GetHitDamage());
	_otherEntity->SendMsg(ptrNewDamageMakeMsg);

}
#pragma endregion
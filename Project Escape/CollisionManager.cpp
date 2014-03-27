#include "CollisionManager.h"
#include "Button.h"
#include "Bullet.h"
#include "Item.h"
#include "SoundRipple.h"
#include "Door.h"

namespace esc
{
	CollisionManager::CollisionManager()
	{
		m_vRemoveVector = new std::vector<GameObject*>();
	}

	void CollisionManager::getCollision(std::vector<GameObject*> *p_collisionObjects)
	{
		bool bDelete = false;

		std::vector<GameObject*> vRemoveVector;

		for (auto object : *p_collisionObjects)
		{
			if (object->getType() == PLAYER)
			{
				for (auto object2 : *p_collisionObjects)
				{
					if (object2->getType() != PLAYER)
					{
						int pPosX = object->getPosition().x;
						int pPosY = object->getPosition().y;
						int pSizeX = object->getSize().x;
						int pSizeY = object->getSize().y;

						int oPosX = object2->getPosition().x;
						int oPosY = object2->getPosition().y;
						int oSizeX = object2->getSize().x;
						int oSizeY = object2->getSize().y;

						float A = pSizeX * 0.5;
						float B = oSizeX * 0.5f;
						float C = (pPosX + A) - (oPosX + B);

						if (fabs(C) < A + B)
						{
							float Q = pSizeY * 0.5;
							float P = oSizeY * 0.5f;
							float Z = (pPosY + A) - (oPosY + B);
							if (fabs(Z) < Q + P)
							{
								PlayerObject *pObj = static_cast<PlayerObject*>(object);
								if (pObj->HandleCollision(object2))
									bDelete = true;
							}
						}
					}
				}
			}
			else if (object->getType() == BULLET)
			{
				for (auto object2 : *p_collisionObjects)
				{
					if (object2->getType() != PLAYER && object2->getType() != BULLET)
					{
						int pPosX = object->getPosition().x;
						int pPosY = object->getPosition().y;
						int pSizeX = object->getSize().x;
						int pSizeY = object->getSize().y;

						int oPosX = object2->getPosition().x;
						int oPosY = object2->getPosition().y;
						int oSizeX = object2->getSize().x;
						int oSizeY = object2->getSize().y;

						float A = pSizeX * 0.5;
						float B = oSizeX * 0.5f;
						float C = (pPosX + A) - (oPosX + B);

						if (fabs(C) < A + B)
						{
							float Q = pSizeY * 0.5;
							float P = oSizeY * 0.5f;
							float Z = (pPosY + A) - (oPosY + B);
							if (fabs(Z) < Q + P)
							{
								Bullet *bullet = static_cast<Bullet*>(object);
								if (bullet->HandleCollision(object2, &vRemoveVector))
									bDelete = true;
							}
						}
					}
				}
			}
			else if (object->getType() == BATON || object->getType() == GUN || object->getType() == RADIO)
			{
				for (auto object2 : *p_collisionObjects)
				{
					if (object2->getType() != PLAYER && object2->getType() != GUN && object2->getType() != BATON &&object2->getType() != RADIO)
					{
						int pPosX = object->getPosition().x;
						int pPosY = object->getPosition().y;
						int pSizeX = object->getSize().x;
						int pSizeY = object->getSize().y;

						int oPosX = object2->getPosition().x;
						int oPosY = object2->getPosition().y;
						int oSizeX = object2->getSize().x;
						int oSizeY = object2->getSize().y;

						float A = pSizeX * 0.5;
						float B = oSizeX * 0.5f;
						float C = (pPosX + A) - (oPosX + B);

						if (fabs(C) < A + B)
						{
							float Q = pSizeY * 0.5;
							float P = oSizeY * 0.5f;
							float Z = (pPosY + A) - (oPosY + B);
							if (fabs(Z) < Q + P)
							{
								if (object->getType() == GUN)
								{
									Item *gun = static_cast<Item*>(object);
									gun->HandleCollision(object2, &vRemoveVector);
									bDelete = true;
								}
								else if (object->getType() == BATON)
								{
									Item *baton = static_cast<Item*>(object);
									baton->HandleCollision(object2, &vRemoveVector);
									bDelete = true;
								}
								else if (object->getType() == RADIO)
								{
									Item *radio = static_cast<Item*>(object);
									radio->HandleCollision(object2, &vRemoveVector);
									bDelete = true;
								}
							}
						}
					}
				}
			}
			else if (object->getType() == PATROLLINGGUARD || object->getType() == STATIONARYGUARD)
			{
				for (auto object2 : *p_collisionObjects)
				{
					if (object2->getType() == DOOR)
					{
						int pPosX = object->getPosition().x;
						int pPosY = object->getPosition().y;
						int pSizeX = object->getSize().x;
						int pSizeY = object->getSize().y;

						int oPosX = object2->getPosition().x;
						int oPosY = object2->getPosition().y;
						int oSizeX = object2->getSize().x;
						int oSizeY = object2->getSize().y;

						float A = pSizeX * 0.5;
						float B = oSizeX * 0.5f;
						float C = (pPosX + A) - (oPosX + B);

						if (fabs(C) < A + B)
						{
							float Q = pSizeY * 0.5;
							float P = oSizeY * 0.5f;
							float Z = (pPosY + A) - (oPosY + B);
							if (fabs(Z) < Q + P)
							{
								Door *door = dynamic_cast<Door*>(object2);

								if (door != nullptr)
								{
									door->HandleCollision(object);
								}
							}
						}
					}
				}
			}
			else if (object->getType() == SOUNDRIPPLE)
			{
				for (auto object2 : *p_collisionObjects)
				{
					if (object2->getType() == PATROLLINGGUARD || object2->getType() == STATIONARYGUARD)
					{
						int pPosX = object->getPosition().x;
						int pPosY = object->getPosition().y;
						int pSizeX = object->getSize().x;
						int pSizeY = object->getSize().y;

						int oPosX = object2->getPosition().x;
						int oPosY = object2->getPosition().y;
						int oSizeX = object2->getSize().x;
						int oSizeY = object2->getSize().y;

						float A = pSizeX * 0.5;
						float B = oSizeX * 0.5f;
						float C = (pPosX + A) - (oPosX + B);

						if (fabs(C) < A + B)
						{
							float Q = pSizeY * 0.5;
							float P = oSizeY * 0.5f;
							float Z = (pPosY + A) - (oPosY + B);
							if (fabs(Z) < Q + P)
							{
								

								Guard *ripple = dynamic_cast<Guard*>(object2);

								if (ripple != nullptr)
								{
									ripple->HandleCollision(object);
								}

								
							}
						}
					}
				}
			}
		}

		if (!bDelete)
			return;

		for (auto object : vRemoveVector)
		{
			for (auto it = p_collisionObjects->begin(); it != p_collisionObjects->end(); ++it)
			{
				if (object == *it)
				{
					delete object;
					object = nullptr;

					p_collisionObjects->erase(it);
					break;
				}
			}
		}

		vRemoveVector.clear();
	}

	void CollisionManager::getCollisionWithPoint(std::vector<GameObject*> *p_collisionObjects, sf::Vector2i p_v2fPoint)
	{
		for (auto object : *p_collisionObjects)
		{
			if (object->getType() == BUTTON)
			{
				int pPosX = object->getPosition().x;
				int pPosY = object->getPosition().y;
				int pSizeX = object->getSize().x;
				int pSizeY = object->getSize().y;

				int oPosX = p_v2fPoint.x;
				int oPosY = p_v2fPoint.y;

				if (oPosX > pPosX && oPosX < pPosX + pSizeX)
				{
					if (oPosY > pPosY && oPosY < pPosY + pSizeY)
					{
						Button *button = static_cast<Button*>(object);
						button->HandleCollision();
						return;
					}
				}

			}
		}
	}
}
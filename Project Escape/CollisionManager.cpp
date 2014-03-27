#include "CollisionManager.h"
#include "Button.h"
#include "Bullet.h"
#include "Item.h"
#include "SoundRipple.h"
#include "Corner.h"
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
						float pPosX = object->getPosition().x;
						float pPosY = object->getPosition().y;
						

						float oPosX = object2->getPosition().x;
						float oPosY = object2->getPosition().y;
						

						float diffX = fabs(pPosX - oPosX);
						float diffY = fabs(pPosY - oPosY);

						SoundRipple *ripple = static_cast<SoundRipple*>(object);

						float rippleRad = ripple->getCurrentRadious();

						float minCollisionDiff = rippleRad + 45;

						float distance = sqrtf(diffX * diffX + diffY * diffY);

						if (minCollisionDiff > distance)
						{
							Guard *guard = static_cast<Guard*>(object2);

							guard->HandleCollision(ripple);
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
<<<<<<< HEAD
						return;
=======
						
>>>>>>> 45c0612b50f424944da1b53a50684c4efcb4882c
					}
				}

			}
		}
	}

	bool CollisionManager::lineCollision(sf::Vector2f A1, sf::Vector2f A2, sf::Vector2f B1, sf::Vector2f B2, double out)
	{
		sf::Vector2f a(A2 - A1);
		sf::Vector2f b(B2 - B1);

		double f = perpDot(a, b);

		if (!f)
			return false;

		sf::Vector2f c(B2 - A2);

		double aa = perpDot(a, c);
		double bb = perpDot(b, c);

		if (f < 0)
		{
			if (aa > 0)     return false;
			if (bb > 0)     return false;
			if (aa < f)     return false;
			if (bb < f)     return false;
		}
		else
		{
			if (aa < 0)     return false;
			if (bb < 0)     return false;
			if (aa > f)     return false;
			if (bb > f)     return false;
		}

		return true;
	}

	bool CollisionManager::getCollisionWithLine(std::vector<GameObject*> *p_vCollisionObjects, sf::Vector2f p_v2iPoint1, sf::Vector2f p_v2iPoint2)
	{
		for (auto object : *p_vCollisionObjects)
		{
			if (object->getType() == WALL)
			{
				int pPosX = object->getPosition().x;
				int pPosY = object->getPosition().y;
				int pSizeX = object->getSize().x;
				int pSizeY = object->getSize().y;

				int oPosX = p_v2iPoint1.x;
				int oPosY = p_v2iPoint1.y;
				int oSizeX = abs(p_v2iPoint1.x - p_v2iPoint2.x);
				int oSizeY = abs(p_v2iPoint1.y - p_v2iPoint2.y);

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
						std::vector<std::vector<sf::Vector2f*>> vLines;

						for (int i = 0; i < 4; i++)
						{
							std::vector<sf::Vector2f*> vTempVector;

							vTempVector.push_back(nullptr);
							vTempVector.push_back(nullptr);

							vLines.push_back(vTempVector);
						}

						std::vector<Corner*> vCorners = object->getCorners();

						int i = 0;

						std::vector<std::vector<sf::Vector2f*>> vNewLines;

						for (auto line : vLines)
						{
							if (i != 3)
							{
								line[0] = &vCorners[i]->position;
								line[1] = &vCorners[++i]->position;
							}
							else
							{
								line[0] = &vCorners[i]->position;
								line[1] = &vCorners[0]->position;
							}

							vNewLines.push_back(line);
						}

						bool hasCollided = false;
						
						for (auto line : vNewLines)
						{
							hasCollided = lineCollision(*line[0], *line[1], p_v2iPoint1, p_v2iPoint2, 0);

							if (hasCollided)
								return true;
						}
					}
				}
			}
		}

		return false;
	}

	double CollisionManager::dot(sf::Vector2f a, sf::Vector2f b)
	{
		return (a.x * b.x) + (a.y * b.y);
	}

	double CollisionManager::perpDot(sf::Vector2f a, sf::Vector2f b)
	{
		return (a.y * b.x) - (a.x * b.y);
	}
}
#include "Door.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "SpriteManager.h"

namespace esc
{
	Door::Door(sf::Vector2f p_v2Pos, sf::Vector2f p_v2Size, bool p_bVertical, int p_iLock, SpriteManager *p_xSpriteManager, int p_iObjId)
		:GameObject(GameObject(p_v2Pos, sf::Vector2f(p_v2Size), true, p_iObjId, DOOR, nullptr))
	{
		m_v2StartPos = p_v2Pos;
		m_v2Pos = p_v2Pos;
		m_v2Size = p_v2Size;

		setOrigin(32, 32);

		m_xSpriteManager = p_xSpriteManager;

		m_fTimeOpen = 0.0f;
		m_fSpeed = 2.0f;
		
		m_bIsClosing = false;
		m_bOpen = false;
		m_bVertical = p_bVertical;
		m_iLock = p_iLock;
	}

	void Door::update(float p_fDtime)
	{
		if (m_bOpen)
		{
			m_fTimeOpen += p_fDtime;

			if (m_bVertical)
			{
				if (getPosition().y < m_v2StartPos.y + 64)
				{
					setPosition(getPosition().x, getPosition().y + m_fSpeed);
				}
				else
				{
					if (m_fTimeOpen > 2.0f)
					{
						m_bOpen = false;
						m_bIsClosing = true;
					}
				}
			}
			else
			{
				if (getPosition().x < m_v2StartPos.x + 64)
				{
					
					setPosition(getPosition().x + m_fSpeed, getPosition().y);
				}
				else
				{
					if (m_fTimeOpen > 2.0f)
					{
						m_bOpen = false;
						m_bIsClosing = true;
					}
				}
			}
		}
		else if (m_bIsClosing)
		{
			if (m_bVertical)
			{
				if (getPosition().y > m_v2StartPos.y)
				{
					setPosition(getPosition().x, getPosition().y - m_fSpeed);
				}
				else
					m_bIsClosing = false;
			}
			else
			{
				if (getPosition().x > m_v2StartPos.x)
				{
					setPosition(getPosition().x - m_fSpeed, getPosition().y);
				}
				else
					m_bIsClosing = false;
			}
			m_fTimeOpen = 0.0f;
		}
		

		if (m_iLock == 1)
		{
			if (m_bVertical == true)
			{
				m_xSprite = m_xSpriteManager->loadSprite("Reddoor2.png", 0, 0, 64, 64);
			}
			else if (m_bVertical == false)
			{
				m_xSprite = m_xSpriteManager->loadSprite("reddoor.png", 0, 0, 64, 64);
			}
		}
		if (m_iLock == 2)
		{
			if (m_bVertical == true)
			{
				m_xSprite = m_xSpriteManager->loadSprite("greydoor2.png", 0, 0, 64, 64);
			}
			else if (m_bVertical == false)
			{
				m_xSprite = m_xSpriteManager->loadSprite("greydoor.png", 0, 0, 64, 64);
			}
		}
		if (m_iLock == 0)
		{
			if (m_bVertical == true)
			{
				m_xSprite = m_xSpriteManager->loadSprite("yellowdoor2.png", 0, 0, 64, 64);
			}
			else if (m_bVertical == false)
			{
				m_xSprite = m_xSpriteManager->loadSprite("yellowdoor.png", 0, 0, 64, 64);
			}
		}
	}

	void Door::HandleCollision(GameObject* p_xObject)
	{
		if (p_xObject->getType() == PATROLLINGGUARD || p_xObject->getType() == STATIONARYGUARD)
		{
			m_bOpen = true;
		}
	}

	void Door::Open()
	{
		m_bOpen = true;
	}

	void Door::unlock()
	{
		m_iLock = 0;
	}

	int Door::IsLocked()
	{
		return m_iLock;
	}

	bool Door::getIsClosing()
	{
		return m_bIsClosing;
	}

	bool Door::IsOpen()
	{
		return m_bOpen;
	}

	bool Door::IsVertical()
	{
		return m_bVertical;
	}
}
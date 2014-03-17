#include "Door.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "SpriteManager.h"

namespace esc
{
	Door::Door(sf::Vector2f p_v2Pos, sf::Vector2f p_v2Size, bool p_bVertical, int p_iLock, SpriteManager *p_xSpriteManager, int p_iObjId)
		:GameObject(GameObject(p_v2Pos, sf::Vector2f(p_v2Size), p_iObjId, true, DOOR, nullptr))
	{
		m_v2Pos = p_v2Pos;
		m_v2Size = p_v2Size;

		m_xSpriteManager = p_xSpriteManager;

		m_bOpen = false;
		m_bVertical = p_bVertical;
		m_iLock = p_iLock;
	}

	void Door::update(float p_fDtime)
	{
		if (m_iLock == 1)
		{
			if (m_bVertical == true)
			{
				m_xSprite = m_xSpriteManager->loadSprite("Reddoor2.png", 26, 0, 12, 64);
			}
			else if (m_bVertical == false)
			{
				m_xSprite = m_xSpriteManager->loadSprite("reddoor.png", 0, 26, 64, 12);
			}
		}
		if (m_iLock == 2)
		{
			if (m_bVertical == true)
			{
				m_xSprite = m_xSpriteManager->loadSprite("greydoor2.png", 26, 0, 12, 64);
			}
			else if (m_bVertical == false)
			{
				m_xSprite = m_xSpriteManager->loadSprite("greydoor.png", 0, 26, 64, 12);
			}
		}
		if (m_iLock == 0)
		{
			if (m_bVertical == true)
			{
				m_xSprite = m_xSpriteManager->loadSprite("yellowdoor2.png", 26, 0, 12, 64);
			}
			else if (m_bVertical == false)
			{
				m_xSprite = m_xSpriteManager->loadSprite("yellowdoor.png", 0, 26, 64, 12);
			}
		}
	}

	void Door::Open()
	{
		if (m_bVertical)
		{
			setPosition(getPosition().x + 32, getPosition().x + 32);
			setRotation(90.0f);
			m_bOpen = true;
			m_bIsRemoved = true;
		}
		else
		{
			setPosition(getPosition().x + 32, getPosition().x + 32);
			setRotation(90.0f);
			m_bOpen = true;
			m_bIsRemoved = true;
		}
	}

	void Door::unlock()
	{
		m_iLock = 0;
	}

	int Door::IsLocked()
	{
		return m_iLock;
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
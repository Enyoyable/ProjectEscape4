#include "GameObjectManager.h"
#include "GameObject.h"
#include "Guard.h"
#include "PlayerObject.h"
#include "Item.h"

namespace esc
{

	GameObjectManager::GameObjectManager(sf::RenderWindow *p_xWindow)
	{
		m_xWindow = p_xWindow;
		m_iCurrentObjectId = 0;
		m_xPlayerObject = nullptr;
	}

	void GameObjectManager::updateObjects(std::vector<GameObject*> *p_vObjects, float p_fDeltaTime)
	{
		for (auto object : *p_vObjects)
		{
			if (object == nullptr)
				continue;

			switch (object->getType())
			{
			case EObjectType::PLAYER:
				static_cast<PlayerObject*>(object)->update(p_fDeltaTime, *p_vObjects);
				break;

			case EObjectType::PATROLLINGGUARD:
				static_cast<Guard*>(object)->update(p_fDeltaTime);
				break;

			case EObjectType::STATIONARYGUARD:
				static_cast<Guard*>(object)->update(p_fDeltaTime);
				break;
			}

			object->update(p_fDeltaTime);
		}
	}

	void GameObjectManager::drawObjects(std::vector<GameObject*> *p_vObjects)
	{
		for (auto object : *p_vObjects)
		{
			m_xWindow->draw(*object);
		}
	}

	void GameObjectManager::removeObject(std::vector<GameObject*> *p_vObjects, GameObject *p_xObject)
	{
		for (auto iter = p_vObjects->begin(); iter != p_vObjects->end(); ++iter)
		{
			if (*iter == p_xObject)
			{
				delete *iter;
				*iter = nullptr;
				p_vObjects->erase(iter);
				return;
			}
		}
	}

	void GameObjectManager::cleanObjects(std::vector<GameObject*> *p_vObjects)
	{
		for (auto object : *p_vObjects)
		{
			delete object;
			object = nullptr;
		}

		p_vObjects->clear();
	}

	GameObject *GameObjectManager::createObject(sf::Vector2f p_v2fPosition, sf::Vector2f p_v2fSize, bool p_bInteractable, EObjectType p_eType, sf::Sprite *p_xSprite)
	{
		return new GameObject(p_v2fPosition, p_v2fSize, p_bInteractable, m_iCurrentObjectId++, p_eType, p_xSprite);
	}

	Guard *GameObjectManager::createGuard(sf::Vector2f p_v2fPosition, sf::Vector2f p_v2fSize, bool p_bInteractable, sf::Sprite *p_xSprite, Level *p_xLevel)
	{
		return new Guard(p_v2fPosition, p_v2fSize, p_bInteractable, m_iCurrentObjectId++, m_xPlayerObject, this, p_xLevel, p_xSprite);
	}

	PlayerObject *GameObjectManager::createPlayer(sf::Sprite *p_sprite, sf::Vector2f p_position, sf::RenderWindow *p_window, int p_iObjectId, Level *level, sf::Clock *p_xTimer)
	{
		if (m_xPlayerObject != nullptr)
		{
			delete m_xPlayerObject;
			m_xPlayerObject = nullptr;
		}

		m_xPlayerObject = new PlayerObject(p_sprite, p_position, p_window, m_iCurrentObjectId++, level, p_xTimer, this);
		return m_xPlayerObject;
	}

	Item *GameObjectManager::createItem(sf::Vector2f p_v2fPosition, sf::Vector2f p_v2fSize, EObjectType p_eType, sf::Sprite *p_xSprite)
	{
		//kommentar
		return new Item(p_v2fPosition, p_xSprite, false, sf::Vector2f(0.0f, 0.0f), m_iCurrentObjectId++, p_eType);
	}
}
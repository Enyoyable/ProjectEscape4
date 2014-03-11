#pragma once

#include "stdafx.h"
#include "GameObject.h"

namespace esc
{
	class GameObject;
	class Guard;
	class PlayerObject;
	class Level;
	class Item;
	class Animator;
	class PathFind;

	class GameObjectManager
	{

		friend class MenuState;
		friend class Engine;

	public:
		GameObjectManager(sf::RenderWindow *p_xWindow);

		void updateObjects(std::vector<GameObject*> *p_vObjects, float p_fDeltaTime);

		void drawObjects(std::vector<GameObject*> *p_vObjects);

		void removeObject(std::vector<GameObject*> *p_vObjects, GameObject *p_xObject);

		void cleanObjects(std::vector<GameObject*> *p_vObjects);

		GameObject *createObject(sf::Vector2f p_v2fPosition, sf::Vector2f p_v2fSize, bool p_bInteractable, EObjectType p_eType, sf::Sprite *p_xSprite);

		Guard *createGuard(sf::Vector2f p_v2fPosition, sf::Vector2f p_v2fSize, bool p_bInteractable, sf::Sprite *p_xSprite, Level *p_xLevel);

		PlayerObject *createPlayer(Animator *p_xAnimator, sf::Sprite *p_sprite, sf::Vector2f p_position, sf::RenderWindow *p_window, int p_iObjectId, Level *level, sf::Clock *p_xTimer);

		Item *createItem(sf::Vector2f p_v2fPosition, sf::Vector2f p_v2fSize, EObjectType p_eType, sf::Sprite *p_xSprite);

	private:
		sf::RenderWindow *m_xWindow;

		PlayerObject *m_xPlayerObject;

		int m_iCurrentObjectId;
	};

}
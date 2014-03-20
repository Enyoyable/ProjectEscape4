#pragma once

#include "stdafx.h"
#include "GameObject.h"

namespace esc
{
	
	class GameObject;
	class DrawManager;
	class Level;
	class Animator;
	class SpriteManager;
	class GameObjectManager;

	class Door : public GameObject
	{
	public:
		Door(sf::Vector2f p_v2Pos, sf::Vector2f p_v2Size, bool p_bVertical, int p_iLock, SpriteManager *p_xSpriteManager, int p_iObjId);

		virtual void update(float p_fDeltaTime);
		void HandleCollision(GameObject* p_xObject);

		void Open();
		void unlock();

		bool IsOpen();
		bool getIsClosing();
		bool IsVertical();
		int IsLocked();


	private:

		sf::Vector2f m_v2StartPos;
		sf::Vector2f m_v2Pos;
		sf::Vector2f m_v2Size;

		SpriteManager *m_xSpriteManager;

		bool m_bOpen;
		bool m_bIsClosing;
		bool m_bVertical;
		int m_iLock;
		float m_fTimeOpen;
		float m_fSpeed;
	};

}
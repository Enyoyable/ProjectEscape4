#pragma once

#include "stdafx.h"
#include "GameObject.h"

namespace esc
{

	class DrawManager;
	class Level;
	class Animator;
	class GameObjectManager;
	class Weapon;
	class StateManager;

	class PlayerObject : public GameObject
	{

		friend class Baton;
		friend class Garrote;

	public:
		PlayerObject(Animator *p_xAnimator, sf::Sprite *p_sprite, sf::Vector2f p_position, sf::RenderWindow *p_window, int p_iObjectId, Level *p_xLevel, sf::Clock *p_xTimer, GameObjectManager *p_xGobjManager, StateManager *p_xStateManager);

		virtual void update(float deltaTime, std::vector<GameObject*> Hideableobjects);

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		void setInteractionRange(float range);

		void reset();

		bool HandleCollision(GameObject *p_oObject);

		void Attack();

		void Throw();

		void Interract(GameObject *p_xInteractObj);

		sf::Vector2f getRangeToObject(GameObject *obj);

		int collisionCheck(sf::Sprite player, sf::Sprite object);
		
		float calcAngle(float x, float y);
		
		float getNoise();
		float getAttRad();
		sf::Vector2f getAttPos();

		sf::CircleShape* getattack();

		void setcurwep(int p_inewWep);
		int getCurWep();

		void setHasCard(bool p_bHascard);
		bool getHasCard();

		void setHasObj(bool p_bHasObj);
		bool getHasObj();

		bool m_hiding;
		bool m_sneaking;

		bool m_bRblock, m_bLblock, m_bUblock, m_bDblock;

		float m_interactionRange;

		std::vector<sf::Vector2f> m_vPath;

		void setStateObjects(std::vector<GameObject*> *p_vStateObjects);

		Weapon *m_xWeapon;

		bool m_bHasCard;

	protected:
		
		int m_iCurWep;
		float m_walkspeed = 2.f;
		float m_sneakspeed = 2.5f;
		float m_Walknoise = 60.0f;
		float m_fCurrentnoise;
		float m_fTimehidden;
		float m_fTimeUnhidden;
		sf::Vector2f m_velocity;
		sf::Vector2f m_startpos;
		sf::RenderWindow *p_window;
		sf::CircleShape *m_noiseCir;
		sf::CircleShape *m_rAttack;

		GameObjectManager *m_xGobjManager;

		std::vector<GameObject*> *m_vStateObjects;
		std::vector<GameObject*> *m_vRemoveObjects;

		
		bool m_bHasObj;
		bool m_bTutComplete;

		Level *m_xLevel;
		sf::Clock *m_xTimer;

		Animator *m_xAnimator;

		sf::Music *m_sStepMusic;

		StateManager *m_xStateManager;
	};

}
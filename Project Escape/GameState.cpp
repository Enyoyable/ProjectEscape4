#include "GameState.h"
#include "GameObjectManager.h"
#include "StateManager.h"
#include "Engine.h"
#include "SpriteManager.h"
#include "PlayerObject.h"
#include "CollisionManager.h"
#include "Level.h"
#include "Weapon.h"
#include "Baton.h"
#include "Gun.h"
#include "Garrote.h"
#include "AnimatedSprite.h"
#include "SoundManager.h"
#include "Animator.h"
#include "AIManager.h"
#include "PathFind.h"
#include "SoundRipple.h"
#include "HUD.h"

namespace esc
{

	GameState::GameState(GameObjectManager *p_xGameObjectManager, StateManager *p_xStateManager, SpriteManager *p_xSpriteManager, Engine *p_xEngine, CollisionManager *p_xCollisionManager, Level *p_xLevel)
	{
		m_xGameObjectManager = p_xGameObjectManager;
		m_xStateManager = p_xStateManager;
		m_xSpriteManager = p_xSpriteManager;
		m_xEngine = p_xEngine;
		m_xLevel = p_xLevel;
		m_fRippleTimer = 0;
		m_tTimer = sf::Time();
	}

	void GameState::init()
	{
		sf::Clock *xTimer = new sf::Clock;
		m_cClock = sf::Clock();
		//m_xPlayer = m_xGameObjectManager->createPlayer(m_xSpriteManager->loadSprite("spy.txt"), sf::Vector2f(64 * 48, 64 * 8), m_xEngine->m_window, 1, m_xLevel, xTimer);
		m_xPlayer = m_xGameObjectManager->createPlayer(new Animator(m_xSpriteManager, "../resources/Spritesheets/"), m_xSpriteManager->loadAnimatedSprite("Spy_walk.txt"), sf::Vector2f(64 * 48, 64 * 8), m_xEngine->m_window, 1, m_xLevel, xTimer);
		m_vGameObjects[MAIN].push_back(m_xPlayer);

		m_vGameObjects[MAIN].insert(m_vGameObjects[MAIN].end(), m_xLevel->getObjects()->begin(), m_xLevel->getObjects()->end());

		m_xView = new sf::View();
		//kommentar
		m_xView->reset(sf::FloatRect(0, 0, 1920 * 1, 1080 * 1));

		m_xPlayer->m_xWeapon = new Gun(true, 2, 1.f, 1.f, &m_vGameObjects[MAIN], m_xGameObjectManager, m_xSpriteManager);
		m_xPlayer->m_xWeapon->setAttachedObject(m_xPlayer);

		/*m_xPlayer->m_xWeapon = new Baton(true, 10, 1.f, 1.f, &m_vGameObjects[MAIN], m_xLevel, m_xGameObjectManager, m_xSpriteManager);
		m_xPlayer->m_xWeapon->setAttachedObject(m_xPlayer);*/

		/*m_xPlayer->m_xWeapon = new Garrote(1.f, 3.f, &m_vGameObjects[MAIN], m_xGameObjectManager, m_xSpriteManager);
		m_xPlayer->m_xWeapon->setAttachedObject(m_xPlayer);*/

		SoundManager soundmanager("../resources/Music/");
		m_sIngame = soundmanager.getMusic("Ingame.ogg");
		m_sIngame->setVolume(40.0f);
		m_sIngame->setLoop(true);
		m_sIngame->play();

		hud = new HUD(m_xView, m_xSpriteManager, m_xPlayer);

		PathFind *path = new PathFind(m_xLevel);

		for (auto obj : m_vGameObjects[MAIN])
		{
			if (obj->getType() == PATROLLINGGUARD || obj->getType() == STATIONARYGUARD)
			{
				Guard *guard = static_cast<Guard*>(obj);

				guard->attachAi(new AIManager(guard, path, m_xPlayer));

				if (guard->getIsRotating())
					guard->m_xAIManager->forceCurrentState(AIManager::STATIONARY);
				else if (guard->getIsPatrolling())
					guard->m_xAIManager->forceCurrentState(AIManager::PATROLLING);
			}
		}

		m_xPlayer->setStateObjects(&m_vGameObjects[MAIN]);
	}

	void GameState::update(float p_fDeltaTime)
	{
		m_fRippleTimer += p_fDeltaTime;
		if (m_bHasInitialized == false)
		{
			m_bHasInitialized = true;
			return;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
		{
			for (auto vGameObjects : m_vGameObjects)
			{
				m_xGameObjectManager->updateObjects(&vGameObjects, p_fDeltaTime);
			}
		}
		
		for (auto vGameObjects : m_vGameObjects)
		{
				m_xGameObjectManager->updateObjects(&vGameObjects, p_fDeltaTime);
		}

		m_xCollisionManager->getCollision(&m_vGameObjects[MAIN]);

		sf::Listener::setPosition(m_xPlayer->getPosition().x, m_xPlayer->getPosition().y, 0);

		m_xView->setCenter(m_xPlayer->getPosition());

		m_xEngine->m_window->setView(*m_xView);

		m_tTimer = m_cClock.getElapsedTime();
		hud->update(p_fDeltaTime, m_tTimer);


		for (auto iter = m_vGameObjects[MAIN].begin(); iter != m_vGameObjects[MAIN].end(); )
		{
			GameObject *obj = *iter;

			if (obj->getType() == SOUNDRIPPLE)
			{
				SoundRipple *ripple = static_cast<SoundRipple*>(obj);

				if (!ripple->m_bIsUpdating)
				{
					delete ripple;
					ripple = nullptr;
					iter = m_vGameObjects[MAIN].erase(iter);
					continue;
				}
			}
			++iter;
		}
		
		if (m_xLevel->getStateNum() != 1)
		{
			if (m_xLevel->getStateNum() == 0)
			{
				m_xStateManager->setCurrentState(StateManager::MENU);
				exit();
			}
			else if (m_xLevel->getStateNum() == 2)
			{
				m_xStateManager->setCurrentState(StateManager::LOSE);
				exit();
			}
			else if (m_xLevel->getStateNum() == 3)
			{
				m_xStateManager->setCurrentState(StateManager::WIN);
				exit();
			}
		}
	}

	void GameState::draw()
	{
		m_xEngine->m_window->draw(*m_xLevel);

		for (auto vGameObjects : m_vGameObjects)
		{
			m_xGameObjectManager->drawObjects(&vGameObjects);
		}

		m_xEngine->m_window->draw(*hud);
	}

	void GameState::exit()
	{
		for (auto vGameObjects : m_vGameObjects)
		{
			m_xGameObjectManager->cleanObjects(&vGameObjects);
		}
	}

	void GameState::pause()
	{

	}


}
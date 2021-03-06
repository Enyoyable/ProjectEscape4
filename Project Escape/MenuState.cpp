#include "MenuState.h"
#include "GameObjectManager.h"
#include "StateManager.h"
#include "Button.h"
#include "SpriteManager.h"
#include "Engine.h"
#include "CollisionManager.h"
#include "AnimatedSprite.h"
#include "SoundManager.h"
#include "GameObject.h"
#include "GameState.h"

namespace esc
{

	MenuState::MenuState(GameObjectManager *p_xGameObjectManager, StateManager *p_xStateManager, SpriteManager *p_xSpriteManager, Engine *p_xEngine, CollisionManager *p_xCollisionManager)
	{
		m_xGameObjectManager = p_xGameObjectManager;
		m_xStateManager = p_xStateManager;
		m_xSpriteManager = p_xSpriteManager;
		m_xEngine = p_xEngine;
		m_xCollisionManager = p_xCollisionManager;
	}

	void MenuState::init()
	{
		m_xMenuBack = m_xGameObjectManager->createObject(sf::Vector2f(0, 0), sf::Vector2f(1920, 1080), false, BACKGROUNDOBJ, m_xSpriteManager->loadSprite("BGTitle.png", 0, 0, 1920, 1080));

		m_xStartButton = new Button(sf::Vector2f(615, 452), sf::Vector2f(715, 126), false, 0, m_xSpriteManager->loadSprite("NG1.png", 0, 0, 715, 126));
		m_xStartButton->setHoverSprite(m_xSpriteManager->loadAnimatedSprite("NewGameAnim.txt"));
		m_xStartButton->setFunction([&](){
			m_xStateManager->detachState(StateManager::EStates::GAME);
			m_xStateManager->attachState(StateManager::EStates::GAME, new GameState(m_xGameObjectManager, m_xStateManager, m_xSpriteManager, m_xEngine, m_xCollisionManager));
			m_xStateManager->setCurrentState(StateManager::EStates::GAME);
			m_sTitle->stop();
			exit();
		});

		m_xExitButton = new Button(sf::Vector2f(615, 602), sf::Vector2f(715, 126), false, 0, m_xSpriteManager->loadSprite("Exit1.png", 0, 0, 715, 126));

		m_xExitButton->setFunction([&](){
			m_xEngine->m_window->close();
		});

		m_xExitButton->setHoverSprite(m_xSpriteManager->loadAnimatedSprite("ExitGameAnim.txt"));

		m_xExitButton->setOrigin(0, 0);

		m_vGameObjects[MAIN].push_back(m_xMenuBack);
		m_vGameObjects[MAIN].push_back(m_xStartButton);
		m_vGameObjects[MAIN].push_back(m_xExitButton);


		m_xSoundManager = new SoundManager("../resources/Music/");
		m_sTitle = m_xSoundManager->getMusic("Title_screen.ogg");
		m_sTitle->setVolume(40.0f);
		m_sTitle->setLoop(true);
		m_sTitle->play();

	}

	void MenuState::update(float p_fDeltaTime)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::G))
		{
			m_xStateManager->setCurrentState(StateManager::EStates::GAME);
			m_sTitle->stop();
		}
		
		if (m_vGameObjects != nullptr)
		{
			for (auto vGameObjects : m_vGameObjects)
			{
				for (auto object : vGameObjects)
				{
					m_xGameObjectManager->updateObjects(&vGameObjects, p_fDeltaTime);

				}
				
			}
		}

		m_xCollisionManager->getCollisionWithPoint(&m_vGameObjects[MAIN], sf::Mouse::getPosition(*m_xEngine->m_window));

		m_xEngine->m_window->setView(m_xEngine->m_window->getDefaultView());
	}

	void MenuState::draw()
	{
		for (auto vGameObjects : m_vGameObjects)
		{
			for (auto object : vGameObjects)
				m_xGameObjectManager->drawObjects(&vGameObjects);
		}

		for (auto text : m_vTextObjects)
		{
			//m_xGameObjectManager->m_xWindow->draw(*text);
		}
	}

	void MenuState::exit()
	{
	

		m_vGameObjects[BACKGROUND].clear();
		m_vGameObjects[MAIN].clear();
		m_vGameObjects[FOREGROUND].clear();
	}

	void MenuState::pause()
	{

	}

}
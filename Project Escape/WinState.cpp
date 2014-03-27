#include "MenuState.h"
#include "LoseState.h"
#include "WinState.h"
#include "GameObjectManager.h"
#include "StateManager.h"
#include "Button.h"
#include "SpriteManager.h"
#include "Engine.h"
#include "CollisionManager.h"
#include "AnimatedSprite.h"
#include "SoundManager.h"
#include "GameObject.h"

namespace esc
{
	WinState::WinState(GameObjectManager *p_xGameObjectManager, StateManager *p_xStateManager, SpriteManager *p_xSpriteManager, Engine *p_xEngine, CollisionManager *p_xCollisionManager)
	{
		m_xGameObjectManager = p_xGameObjectManager;
		m_xStateManager = p_xStateManager;
		m_xSpriteManager = p_xSpriteManager;
		m_xEngine = p_xEngine;
		m_xCollisionManager = p_xCollisionManager;
	}

	void WinState::init()
	{
		m_xWinBack = m_xGameObjectManager->createObject(sf::Vector2f(0, 0), sf::Vector2f(1920, 1080), false, BACKGROUNDOBJ, m_xSpriteManager->loadSprite("BGWin.png", 0, 0, 1920, 1080));

		m_xMenuButton = new Button(sf::Vector2f(10, 242), sf::Vector2f(715, 126), false, 0, m_xSpriteManager->loadSprite("MenuButton.png", 0, 0, 715, 126));

		m_xMenuButton->setFunction([&](){
			m_xStateManager->setCurrentState(StateManager::EStates::MENU);
			//m_sGameOver->stop();
			exit();
		});

		m_xMenuButton->setHoverSprite(m_xSpriteManager->loadAnimatedSprite("MenuAnim.txt"));

		m_xMenuButton->setOrigin(0, 0);

		m_xRestartButton = new Button(sf::Vector2f(10, 368), sf::Vector2f(715, 126), false, 0, m_xSpriteManager->loadSprite("RestartButton.png", 0, 0, 715, 126));

		m_xRestartButton->setFunction([&](){
			m_xStateManager->setCurrentState(StateManager::EStates::GAME);
			//m_sGameOver->stop();
			exit();
		});

		m_xRestartButton->setHoverSprite(m_xSpriteManager->loadAnimatedSprite("RestartAnim.txt"));

		m_xRestartButton->setOrigin(0, 0);

		m_xExitButton = new Button(sf::Vector2f(10, 494), sf::Vector2f(715, 126), false, 0, m_xSpriteManager->loadSprite("Exit1.png", 0, 0, 715, 126));

		m_xExitButton->setFunction([&](){
			m_xEngine->m_window->close();
		});

		m_xExitButton->setHoverSprite(m_xSpriteManager->loadAnimatedSprite("ExitGameAnim.txt"));

		m_xExitButton->setOrigin(0, 0);

		m_vGameObjects[MAIN].push_back(m_xWinBack);
		m_vGameObjects[MAIN].push_back(m_xRestartButton);
		m_vGameObjects[MAIN].push_back(m_xMenuButton);
		m_vGameObjects[MAIN].push_back(m_xExitButton);

		/*m_xSoundManager = new SoundManager("../resources/Music/");
		m_sWin = m_xSoundManager->getMusic("Win.ogg");
		m_sWin->setVolume(40.0f);
		m_sWin->setLoop(true);
		m_sWin->play();*/
	}

	void WinState::update(float p_fDeltaTime)
	{
		for (auto vGameObjects : m_vGameObjects)
		{
			for (auto object : vGameObjects)
			{
				m_xGameObjectManager->updateObjects(&vGameObjects, p_fDeltaTime);

			}
			m_xCollisionManager->getCollisionWithPoint(&vGameObjects, sf::Mouse::getPosition(*m_xEngine->m_window));
		}

		m_xEngine->m_window->setView(m_xEngine->m_window->getDefaultView());
	}

	void WinState::draw()
	{
		for (auto vGameObjects : m_vGameObjects)
		{
			for (auto object : vGameObjects)
				m_xGameObjectManager->drawObjects(&vGameObjects);
		}
	}

	void WinState::exit()
	{
		for (auto vGameObjects : m_vGameObjects)
		{
			m_xGameObjectManager->cleanObjects(&vGameObjects);
		}
	}

	void WinState::pause()
	{

	}
}
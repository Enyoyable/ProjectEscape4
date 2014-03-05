#include "MenuState.h"
#include "GameObjectManager.h"
#include "StateManager.h"
#include "Button.h"
#include "SpriteManager.h"
#include "Engine.h"
#include "CollisionManager.h"
#include "AnimatedSprite.h"

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
		/*sf::Font *font = new sf::Font;

		font->loadFromFile("../Andes.ttf");

		sf::Text *smallTitleText = new sf::Text("Plans of", *font);
		smallTitleText->setCharacterSize(12);
		smallTitleText->setStyle(sf::Text::Regular);
		smallTitleText->setPosition(sf::Vector2f(851, 102));

		sf::Text *largeTitleText = new sf::Text("Futura", *font);
		largeTitleText->setCharacterSize(200);
		largeTitleText->setStyle(sf::Text::Regular);
		largeTitleText->setPosition(sf::Vector2f(597, 202));

		m_vTextObjects.push_back(smallTitleText);
		m_vTextObjects.push_back(largeTitleText);*/

		m_xStartButton = new Button(sf::Vector2f(300, 300), sf::Vector2f(715, 126), false, 0, m_xSpriteManager->loadSprite("NG1.png", 0, 0, 715, 126));

		m_xStartButton->setFunction([&](){
			m_xStateManager->setCurrentState(StateManager::EStates::GAME);
		});

		m_xStartButton->setHoverSprite(m_xSpriteManager->loadAnimatedSprite("NewGameAnim.txt"));

		m_xStartButton->setOrigin(0, 0);

		m_xExitButton = new Button(sf::Vector2f(300, 600), sf::Vector2f(715, 126), false, 0, m_xSpriteManager->loadSprite("Exit1.png", 0, 0, 715, 126));

		m_xExitButton->setFunction([&](){
			m_xEngine->m_window->close();
		});

		m_xExitButton->setHoverSprite(m_xSpriteManager->loadAnimatedSprite("ExitGameAnim.txt"));

		m_xExitButton->setOrigin(0, 0);

		m_vGameObjects[MAIN].push_back(m_xStartButton);
		m_vGameObjects[MAIN].push_back(m_xExitButton);

	}

	void MenuState::update(float p_fDeltaTime)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::G))
		{
			m_xStateManager->setCurrentState(StateManager::EStates::GAME);
		}
		
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
		for (auto vGameObjects : m_vGameObjects)
		{
			m_xGameObjectManager->cleanObjects(&vGameObjects);
		}
	}

	void MenuState::pause()
	{

	}

}
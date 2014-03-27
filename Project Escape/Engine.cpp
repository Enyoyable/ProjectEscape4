#include "Engine.h"
#include "AnimatedSprite.h"
#include "SpriteManager.h"
#include "GameObject.h"
#include "LightSource.h"
#include "Level.h"
#include "StateManager.h"
#include "GameObjectManager.h"
#include "MenuState.h"
#include "GameState.h"
#include "WinState.h"
#include "LoseState.h"
#include "PlayerObject.h"
#include "Guard.h"
#include "CollisionManager.h"
#include "HUD.h"
#include "PathFind.h"
#include "AIManager.h"
#include "Button.h"
#include "Animator.h"

namespace esc
{

	Engine::Engine()
	{

	}

	Engine::~Engine()
	{

	}

	void Engine::initialize()
	{
		sf::ContextSettings settings;
		settings.antialiasingLevel = 8;
		m_window = new sf::RenderWindow(sf::VideoMode(1920, 1080), "Plans of Futura", sf::Style::Fullscreen, settings);
		m_window->setFramerateLimit(60);
		m_window->setVerticalSyncEnabled(true);
	}

	void Engine::run()
	{
		srand(static_cast <unsigned> (time(0)));
		sf::CircleShape circleShape(100.f);
		circleShape.setFillColor(sf::Color::Yellow);

		GameObjectManager *xGameObjectManager = new GameObjectManager(m_window);

		StateManager *xStateManager = new StateManager;

		CollisionManager *xCollisionManager = new CollisionManager();

		sf::Clock clock;

		sf::View view;

		view.reset(sf::FloatRect(0, 0, 1920, 1080));

		SpriteManager spriteManager("../resources/sprites/");

		std::vector<GameObject*> objects;

		sf::Sprite *sprite = spriteManager.loadSprite("spy.txt");

		sf::Clock timer;

		Animator *animator = new Animator(&spriteManager, "../resources/Spritesheets/");

		PlayerObject *pObj = xGameObjectManager->createPlayer(animator, sprite, sf::Vector2f(64 * 17, 64 * 8), m_window, 1, nullptr, &timer, xStateManager);
		objects.push_back(pObj);
		
		
		
		xStateManager->attachState(StateManager::WIN, new WinState(xGameObjectManager, xStateManager, &spriteManager, this, xCollisionManager));
		xStateManager->attachState(StateManager::LOSE, new LoseState(xGameObjectManager, xStateManager, &spriteManager, this, xCollisionManager));
		xStateManager->attachState(StateManager::GAME, new GameState(xGameObjectManager, xStateManager, &spriteManager, this, xCollisionManager));
		xStateManager->attachState(StateManager::MENU, new MenuState(xGameObjectManager, xStateManager, &spriteManager, this, xCollisionManager));
		

		xStateManager->setCurrentState(StateManager::MENU);

		while (m_window->isOpen())
		{

			sf::Event event;

			while (m_window->pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
				{
					m_window->close();
				}

				if (event.key.code == sf::Keyboard::Escape)
				{
					m_window->close();
				}
			}

			float fDeltaTime = clock.restart().asSeconds();

			if (fDeltaTime > 0.1)
				fDeltaTime = 0.1;

			StateManager::EStates firststate = xStateManager->getCurrentState();

			xStateManager->updateCurrentState(fDeltaTime);

			m_window->clear();

			if (xStateManager->getCurrentState() == firststate)
				xStateManager->drawCurrentState();

			m_window->display();
		}
	}

	void Engine::cleanup()
	{

	}

}
#include "HUD.h"
#include "SpriteManager.h"
#include "PlayerObject.h"

namespace esc
{
	HUD::HUD(sf::View *p_xView, SpriteManager *p_xSpriteManager, PlayerObject *p_xObj)
	{
		m_xSpriteManager = p_xSpriteManager;
		m_xPlayerObject = p_xObj;
		m_xView = p_xView;

		m_xLeftSmallCircle = m_xSpriteManager->loadSprite("PHsmall.png", 0, 0, 193, 190);
		m_xLeftSmallCircleItem = m_xSpriteManager->loadSprite("Keycard.png", 0, 0, 91, 81);

		//m_xRightLargeCircle = m_xSpriteManager->loadSprite("Placeholderbig.png", 0, 0, 236, 236);
		//m_xRightLargeCircleItem = m_xSpriteManager->loadSprite("Baton128.png", 0, 0, 140, 152);

		m_xRightLargeCircle = m_xSpriteManager->loadSprite("PH.png", 0, 0, 236, 236);
		m_xRightLargeCircleBaton = m_xSpriteManager->loadSprite("Baton128.png", 0, 0, 236, 236);
		m_xRightLargeCircleGarotte = m_xSpriteManager->loadSprite("Garrote128.png", 0, 0, 236, 236);
		m_xRightLargeCircleGun = m_xSpriteManager->loadSprite("Gun128.png", 0, 0, 236, 236);

		m_xLeftLargeCircle = m_xSpriteManager->loadSprite("PH.png", 0, 0, 236, 236);
		m_xLeftLargeCircleItem = m_xSpriteManager->loadSprite("PDA.png", 0, 0, 236, 236);

		m_xFont = new sf::Font();
		m_xFont->loadFromFile("../Andes.ttf");

		m_xText = new sf::Text("", *m_xFont, 40);
	}

	void HUD::update(float p_fDeltaTime, sf::Time p_xTimer)
	{
		if (m_xPlayerObject->getCurWep() == 1)
		{
			m_xRightLargeCircleItem = m_xRightLargeCircleBaton;
		}
		else if (m_xPlayerObject->getCurWep() == 2)
		{
			m_xRightLargeCircleItem = m_xRightLargeCircleGun;
		}
		else if (m_xPlayerObject->getCurWep() == 0)
		{
			m_xRightLargeCircleItem = m_xRightLargeCircleGarotte;
		}

		sf::Vector2f screensize(1920, 1080);

		m_xLeftSmallCircle->setPosition(m_xView->getCenter() + sf::Vector2f(3, 675) - screensize / 2.f);
		m_xLeftSmallCircleItem->setPosition(m_xView->getCenter() + sf::Vector2f(50, 732) - screensize / 2.f);

		m_xRightLargeCircle->setPosition(m_xView->getCenter() + sf::Vector2f(1690, 847) - screensize / 2.f);
		m_xRightLargeCircleItem->setPosition(m_xView->getCenter() + sf::Vector2f(1690, 847) - screensize / 2.f);

		m_xLeftLargeCircle->setPosition(m_xView->getCenter() + sf::Vector2f(3, 847) - screensize / 2.f);
		m_xLeftLargeCircleItem->setPosition(m_xView->getCenter() + sf::Vector2f(3, 847) - screensize / 2.f);

		std::stringstream sstream;

		sstream << (int)p_xTimer.asSeconds();

		m_xText->setString(sstream.str());
		m_xText->setPosition(m_xView->getCenter() + sf::Vector2f(1920/2 - 20, 50) - screensize / 2.f);
	}

	void HUD::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		target.draw(*m_xLeftLargeCircle, states);
		target.draw(*m_xLeftSmallCircle, states);
		target.draw(*m_xRightLargeCircle, states);

		target.draw(*m_xText);
		
		if (m_xPlayerObject->getHasCard() == true)
			target.draw(*m_xLeftSmallCircleItem, states);

		if (m_xPlayerObject->getHasObj() == true)
			target.draw(*m_xLeftLargeCircleItem, states);
	
		if (m_xRightLargeCircleItem != nullptr)
			target.draw(*m_xRightLargeCircleItem, states);
	}
}
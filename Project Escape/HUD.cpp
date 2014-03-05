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

		m_xLeftSmallCircle = m_xSpriteManager->loadSprite("Placeholdersmall.png", 0, 0, 125, 124);
		m_xLeftSmallCircleItem = m_xSpriteManager->loadSprite("Keycard.png", 0, 0, 91, 81);

		m_xRightLargeCircle = m_xSpriteManager->loadSprite("Placeholderbig.png", 0, 0, 236, 236);
		m_xRightLargeCircleItem = m_xSpriteManager->loadSprite("Baton.png", 0, 0, 140, 152);
		m_xRightLargeCircleBaton = m_xSpriteManager->loadSprite("Baton.png", 0, 0, 140, 152);
		m_xRightLargeCircleGarotte = m_xSpriteManager->loadSprite("Garrote.png", 0, 0, 128, 128);
		m_xRightLargeCircleGun = m_xSpriteManager->loadSprite("Gun.png", 0, 0, 128, 128);

		m_xLeftLargeCircle = m_xSpriteManager->loadSprite("Placeholderbig.png", 0, 0, 236, 236);
		m_xLeftLargeCircleItem = m_xSpriteManager->loadSprite("PDA.png", 0, 0, 186, 205);

		m_xFont = new sf::Font();
		m_xFont->loadFromFile("../Andes.ttf");

		m_xText = new sf::Text("", *m_xFont, 40);
	}

	void HUD::update(float p_fDeltaTime, sf::Time p_xTimer)
	{
		if (m_xPlayerObject->getCurWep() == 1)
		{
			m_xRightLargeCircle = m_xRightLargeCircleBaton;
		}
		else if (m_xPlayerObject->getCurWep() == 2)
		{
			m_xRightLargeCircle = m_xRightLargeCircleGun;
		}
		else if (m_xPlayerObject->getCurWep() == 0)
		{
			m_xRightLargeCircle = m_xRightLargeCircleGarotte;
		}

		sf::Vector2f screensize(1920, 1080);

		m_xLeftSmallCircle->setPosition(m_xView->getCenter() + sf::Vector2f(3, 735) - screensize / 2.f);

		m_xLeftSmallCircleItem->setPosition(m_xView->getCenter() + sf::Vector2f(19, 755) - screensize / 2.f);

		m_xRightLargeCircle->setPosition(m_xView->getCenter() + sf::Vector2f(1690, 847) - screensize / 2.f);

		m_xRightLargeCircleItem->setPosition(m_xView->getCenter() + sf::Vector2f(1735, 890) - screensize / 2.f);

		m_xLeftLargeCircle->setPosition(m_xView->getCenter() + sf::Vector2f(3, 847) - screensize / 2.f);

		m_xLeftLargeCircleItem->setPosition(m_xView->getCenter() + sf::Vector2f(30, 867) - screensize / 2.f);

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
		{
			target.draw(*m_xLeftSmallCircleItem, states);
		}

		if (m_xPlayerObject->getHasObj() == true)
		{
			target.draw(*m_xLeftLargeCircleItem, states);
		}

		if (m_xPlayerObject->getCurWep() == 1)
		{
			target.draw(*m_xRightLargeCircleItem, states);
		}
	}
}
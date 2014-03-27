#pragma once

#include "GameObject.h"
#include "Corner.h"

namespace esc
{

	GameObject::GameObject(sf::Vector2f p_v2fPosition, sf::Vector2f p_v2fSize, bool p_bInteractable, int p_iObjectId, EObjectType p_eType, sf::Sprite *p_xSprite)
	{
		m_xSprite = p_xSprite;

		m_bInteractable = p_bInteractable;

		setPosition(p_v2fPosition);

		m_v2fSize = p_v2fSize;

		m_iObjectId = p_iObjectId;

		m_eObjectType = p_eType;

		setOrigin(32, 32);

		m_bIsRemoved = false;

		m_bAngleLocked = false;
	}

	void GameObject::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (m_bIsRemoved)
		{
			return;
		}
		states.transform *= getTransform();

		if (m_xSprite != nullptr)
		target.draw(*m_xSprite, states);
	}

	void GameObject::update(float deltaTime)
	{
		if (m_bIsRemoved)
			return;
		createCorners();
	}

	void GameObject::setSprite(sf::Sprite *p_xSprite)
	{
		m_xSprite = p_xSprite;
	}

	sf::Sprite *GameObject::getSprite()
	{
		return m_xSprite;
	}

	void GameObject::setSize(sf::Vector2f p_v2fSize)
	{
		m_v2fSize = p_v2fSize;
	}

	sf::Vector2f GameObject::getSize()
	{
		return m_v2fSize;
	}

	void GameObject::setInteractable(bool p_bInteractable)
	{
		m_bInteractable = p_bInteractable;
	}

	bool GameObject::getInteractable()
	{
		return m_bInteractable;
	}

	void GameObject::setTransparent(bool p_bIsTransparent)
	{
		m_bIsTransparent = p_bIsTransparent;
	}

	bool GameObject::getTransparent()
	{
		return m_bIsTransparent;
	}

	void GameObject::createCorners()
	{
		for (auto corner : m_vCorners)
		{
			delete corner;
			corner = nullptr;
		}

		m_vCorners.clear();

		m_vCorners.push_back(new Corner(getPosition() + sf::Vector2f(0.001, 0.001) - getOrigin(), sf::Color(255, 255, 255, 0), m_iObjectId));
		m_vCorners.push_back(new Corner(sf::Vector2f(getPosition().x + m_v2fSize.x + 0.001, getPosition().y - 0.001) - getOrigin(), sf::Color(255, 255, 255, 0), m_iObjectId));
		m_vCorners.push_back(new Corner(sf::Vector2f(getPosition().x + m_v2fSize.x - 0.001, getPosition().y + m_v2fSize.y - 0.001) - getOrigin(), sf::Color(255, 255, 255, 0), m_iObjectId));
		m_vCorners.push_back(new Corner(sf::Vector2f(getPosition().x + 0.001, getPosition().y + m_v2fSize.y - 0.001) - getOrigin(), sf::Color(255, 255, 255, 0), m_iObjectId));
		
	}

	std::vector<Corner*> GameObject::getCorners()
	{
		return m_vCorners;
	}

	void GameObject::setId(int p_iObjectId)
	{
		m_iObjectId = p_iObjectId;
	}

	int GameObject::getId()
	{
		return m_iObjectId;
	}

	void GameObject::setType(EObjectType p_eObjectType)
	{
		m_eObjectType = p_eObjectType;
	}

	EObjectType GameObject::getType()
	{
		return m_eObjectType;
	}

	bool GameObject::isHideable()
	{
		return m_bInteractable;
	}

	void GameObject::HandleCollision(GameObject *p_oGameObject)
	{
		
	}

	void GameObject::setIsRemoved(bool p_bIsRemoved)
	{
		m_bIsRemoved = p_bIsRemoved;
	}

	bool GameObject::getIsRemoved()
	{
		return m_bIsRemoved;
	}
}
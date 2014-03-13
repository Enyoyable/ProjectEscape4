#include "PlayerObject.h"
#include "DrawManager.h"
#include <math.h>
#include "Level.h"
#include "Item.h"
#include "GameObjectManager.h"
#include "SpriteManager.h"
#include "Baton.h"
#include "Weapon.h"
#include "Gun.h"
#include "Garrote.h"
#include "AnimatedSprite.h"
#include "Animator.h"
#include "GameObject.h"
#include "Door.h"


namespace esc
{

	PlayerObject::PlayerObject(Animator *p_xAnimator, sf::Sprite *sprite, sf::Vector2f position, sf::RenderWindow *window, int p_iObjectId, Level *p_xLevel, sf::Clock *p_xTimer, GameObjectManager *p_xGobjManager)
		: GameObject(position, sf::Vector2f(64, 64), false, p_iObjectId, PLAYER, sprite)
	{
		m_hiding = false;
		m_interactionRange = 65.0f;
		setOrigin(32, 32);
		p_window = window;

		m_startpos = position;
		m_walkspeed = 8.0f;
		m_sneakspeed = 4.0f;

		m_iCurWep = 2;
		m_bHasCard = true;

		m_bRblock = false;
		m_bLblock = false;
		m_bUblock = false;
		m_bDblock = false;

		m_noiseCir = new sf::CircleShape(50);
		m_noiseCir->setFillColor(sf::Color(0, 0, 0, 0));
		m_noiseCir->setOutlineThickness(1);
		m_noiseCir->setOutlineColor(sf::Color(250, 150, 100));
		m_noiseCir->setOrigin(50, 50);

		m_eObjectType = PLAYER;
		m_rAttack = nullptr;
		m_xLevel = p_xLevel;
		m_xTimer = p_xTimer;

		m_xGobjManager = p_xGobjManager;

		m_xWeapon = nullptr;

		m_vStateObjects = nullptr;

		m_xAnimator = p_xAnimator;

		m_xAnimator->loadAnimations("playerAnims.txt");

	}

	void PlayerObject::update(float deltaTime, std::vector<GameObject*> objects)
	{
		if (m_xWeapon->getAttachedObject() == nullptr)
			m_xWeapon->setAttachedObject(this);

		if (m_xWeapon != nullptr)
			m_xWeapon->update(deltaTime);

		if (m_rAttack != nullptr)
		{
			delete m_rAttack;
			m_rAttack = nullptr;
		}
		//setPosition(getPosition() + m_velocity * deltaTime);
		m_noiseCir->setPosition(getPosition());
		if (m_noiseCir->getRadius() > 0)
		{
			m_noiseCir->setRadius(m_noiseCir->getRadius() - 1);
			m_noiseCir->setOrigin(m_noiseCir->getOrigin().x - 1, m_noiseCir->getOrigin().x - 1);
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			Attack();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
		{
			if (m_hiding == false)
			{
				for (auto object : objects)
				{
					if (fabs(getPosition().x - (object->getPosition().x)) < m_interactionRange &&
						(fabs(getPosition().y - (object->getPosition().y))) < m_interactionRange &&
						object->getInteractable() == true)
					{
						Interract(object);
					}
				}
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
		{
			if (m_xWeapon == nullptr)
				return;

			m_xWeapon->drop();
			m_xWeapon = new Garrote(0.0f, 1.0f, m_vStateObjects);
			m_iCurWep = 0;
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
		{
			if (m_xWeapon != nullptr)
			{
				m_xWeapon->setTarget(sf::Vector2f(sf::Mouse::getPosition(*p_window)) + getPosition() - sf::Vector2f(960, 540));
				m_xWeapon->Throw();
				m_iCurWep = 0;
				m_xWeapon = nullptr;
				m_xWeapon = new Garrote(1.f, 3.f, m_vStateObjects);
				m_xWeapon->setAttachedObject(this);
			}
		}

		if (m_hiding == false)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			{
				m_sneaking = true;
			}
			else
				m_sneaking = false;

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !m_bUblock)
			{
				if (m_sneaking)
				{
					m_xAnimator->setCurrentAnimation("Spy_sneak.txt");
					move(0, -m_sneakspeed);
				}
				else
				{
					if (m_noiseCir->getRadius() < m_Walknoise)
					{
						m_noiseCir->setRadius(m_noiseCir->getRadius() + 20.0f);
						m_noiseCir->setOrigin(m_noiseCir->getOrigin().x + 20.0f, m_noiseCir->getOrigin().x + 20.0f);
					}
					move(0, -m_walkspeed);
					m_xAnimator->setCurrentAnimation("Spy_walk.txt");
				}
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && !m_bDblock)
			{
				if (m_sneaking)
				{
					move(0, m_sneakspeed);
					m_xAnimator->setCurrentAnimation("Spy_sneak.txt");
				}
					
				else
				{
					if (m_noiseCir->getRadius() < m_Walknoise)
					{
						m_noiseCir->setRadius(m_noiseCir->getRadius() + 20.0f);
						m_noiseCir->setOrigin(m_noiseCir->getOrigin().x + 20.0f, m_noiseCir->getOrigin().x + 20.0f);
					}
					move(0, m_walkspeed);
					m_xAnimator->setCurrentAnimation("Spy_walk.txt");
				}
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && !m_bLblock)
			{
				if (m_sneaking)
				{
					m_xAnimator->setCurrentAnimation("Spy_sneak.txt");
					move(-m_sneakspeed, 0);
				}
				else
				{
					if (m_noiseCir->getRadius() < m_Walknoise)
					{
						m_noiseCir->setRadius(m_noiseCir->getRadius() + 20.0f);
						m_noiseCir->setOrigin(m_noiseCir->getOrigin().x + 20.0f, m_noiseCir->getOrigin().x + 20.0f);
					}
					m_xAnimator->setCurrentAnimation("Spy_walk.txt");
					move(-m_walkspeed, 0);
				}
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && !m_bRblock)
			{
				if (m_sneaking)
				{
					move(m_sneakspeed, 0);
					m_xAnimator->setCurrentAnimation("Spy_sneak.txt");
				}
				else
				{
					if (m_noiseCir->getRadius() < m_Walknoise)
					{
						m_noiseCir->setRadius(m_noiseCir->getRadius() + 20.0f);
						m_noiseCir->setOrigin(m_noiseCir->getOrigin().x + 20.0f, m_noiseCir->getOrigin().x + 20.0f);
					}
					move(m_walkspeed, 0);
					m_xAnimator->setCurrentAnimation("Spy_walk.txt");
				}
			}
			if (!m_bAngleLocked)
			setRotation(calcAngle(sf::Mouse::getPosition(*p_window).x + getPosition().x - p_window->getSize().x * 0.5, sf::Mouse::getPosition(*p_window).y + getPosition().y - p_window->getSize().y * 0.5));
		}

		if (m_hiding == true)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
			{
				m_hiding = false;
				move(0, 0);
			}
		}

		m_bRblock = false;
		m_bLblock = false;
		m_bUblock = false;
		m_bDblock = false;
		
		if (m_xWeapon != nullptr)
			m_xWeapon->update(deltaTime);

		m_xAnimator->update(deltaTime);

		static_cast<AnimatedSprite*>(m_xSprite)->update(deltaTime);
		m_xAnimator->update(deltaTime);
	}

	float PlayerObject::calcAngle(float mouse_x, float mouse_y)
	{
		sf::Vector2f direction(getPosition().x - mouse_x, getPosition().y - mouse_y);

		float angle = (float)(atan2f(direction.y, direction.x) / 0.017453292519943) + 180;

		return angle;
	}

	float PlayerObject::getNoise()
	{
		return m_noiseCir->getRadius();
	}

	float PlayerObject::getAttRad()
	{
		return m_rAttack->getRadius();
	}

	sf::Vector2f PlayerObject::getAttPos()
	{
		return m_rAttack->getPosition();
	}

	void PlayerObject::reset()
	{
		setPosition(m_startpos);
		m_xTimer->restart();
		//m_xWeapon = new Gun(true, 10, 1.f, 1.f, m_xLevel->getObjects(), m_xGobjManager, m_xLevel->getSpriteManager());
		//m_iCurWep = 2;
	}

	void PlayerObject::setcurwep(int newWep)
	{
		m_iCurWep = newWep;
	}

	void PlayerObject::setHasCard(bool newState)
	{
		m_bHasCard = newState;
	}

	void PlayerObject::setHasObj(bool newState)
	{
		m_bHasObj = newState;
	}

	void PlayerObject::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (m_vPath.size() > 0)
		{
			for (auto path : m_vPath)
			{
				sf::CircleShape shape(10, 30);
				shape.setOrigin(10, 10);
				shape.setPosition(path);
				
				shape.setFillColor(sf::Color::Red);

				target.draw(shape);
			}
		}

		states.transform *= getTransform();
		target.draw(*m_noiseCir);
		target.draw(*static_cast<AnimatedSprite*>(m_xAnimator->getCurrentAnimation()), states);
	}

	void PlayerObject::setInteractionRange(float range)
	{
		m_interactionRange = range;
	}

	void PlayerObject::Interract(GameObject *p_xInteractObj)
	{
		if (p_xInteractObj->getType() == LOCKER ||
			p_xInteractObj->getType() == LOCKERD ||
			p_xInteractObj->getType() == LOCKERL ||
			p_xInteractObj->getType() == LOCKERR ||
			p_xInteractObj->getType() == LOCKERU)
		{
			m_hiding = true;
			setPosition(p_xInteractObj->getPosition().x, p_xInteractObj->getPosition().y);
			setRotation(0.0f);
		}
		else if (p_xInteractObj->getType() == BATON)
		{
			if (p_xInteractObj->getIsRemoved() != false)
				return;

			m_xWeapon = nullptr;
			m_xWeapon = new Baton(true, 10, 1.f, 1.f, m_vStateObjects, m_xLevel, m_xGobjManager, m_xLevel->getSpriteManager());
			m_iCurWep = 1;
			p_xInteractObj->setIsRemoved(true);

		}
		else if (p_xInteractObj->getType() == KEYCARD)
		{
			if (p_xInteractObj->getIsRemoved() == true)
				return;

			m_bHasCard = true;
			p_xInteractObj->setIsRemoved(true);
		}
		else if (p_xInteractObj->getType() == PDA)
		{
			if (p_xInteractObj->getIsRemoved() == true)
				return;

			m_bHasObj = true;
			p_xInteractObj->setIsRemoved(true);
		}
		else if (p_xInteractObj->getType() == GUN)
		{
			if (p_xInteractObj->getIsRemoved() == true)
				return;

			m_xWeapon = nullptr;
			m_xWeapon = new Gun(true, 10, 1.f, 1.f, m_vStateObjects, m_xGobjManager, m_xLevel->getSpriteManager());
			m_xWeapon->setAttachedObject(this);
			m_iCurWep = 2;
			p_xInteractObj->setIsRemoved(true);
		}
		else if (p_xInteractObj->getType() == DOOR)
		{
			Door *doorObj = static_cast<Door*>(p_xInteractObj);

			if (doorObj->IsOpen() == false)
			{
				if (doorObj->IsLocked() == 0)
				{
					doorObj->Open();
				}
				if (m_bHasCard == true)
				{
					if (doorObj->IsLocked() == 1)
					{
						doorObj->unlock();
						m_bHasCard = false;
					}
				}
			}
		}
	}

	void PlayerObject::Attack()
	{
		if (m_xWeapon->getCurrentWeaponType() == EWeaponType::WEAPONBATON)
		{
			Baton *baton = static_cast<Baton*>(m_xWeapon);
			baton->trigger();
		}
		else if (m_xWeapon->getCurrentWeaponType() == EWeaponType::WEAPONGUN)
		{
			Gun *gun = static_cast<Gun*>(m_xWeapon);
			gun->setTarget(sf::Vector2f(sf::Mouse::getPosition(*p_window)) + getPosition() - sf::Vector2f(960, 540));
			gun->trigger();
		}
	}

	void PlayerObject::Throw()
	{
		if (m_xWeapon->getCurrentWeaponType() == EWeaponType::WEAPONBATON)
		{
			Baton *baton = static_cast<Baton*>(m_xWeapon);
			baton->setTarget(sf::Vector2f(sf::Mouse::getPosition(*p_window)) + getPosition() - sf::Vector2f(960, 540));
			baton->Throw();
		}
		else if (m_xWeapon->getCurrentWeaponType() == EWeaponType::WEAPONGUN)
		{
			Gun *gun = static_cast<Gun*>(m_xWeapon);
			gun->setTarget(sf::Vector2f(sf::Mouse::getPosition(*p_window)) + getPosition() - sf::Vector2f(960, 540));
			gun->Throw();
		}
		else if (m_xWeapon->getCurrentWeaponType() == EWeaponType::WEAPONGARROTE)
		{
			Garrote *garrote = static_cast<Garrote*>(m_xWeapon);
			garrote->setTarget(sf::Vector2f(sf::Mouse::getPosition(*p_window)) + getPosition() - sf::Vector2f(960, 540));
			garrote->Throw();
		}
	}

	bool PlayerObject::HandleCollision(GameObject *p_oObject)
	{
		if (p_oObject->getType() == WALL)
		{
			float xDiff = p_oObject->getPosition().x - getPosition().x;
			float yDiff = p_oObject->getPosition().y - getPosition().y;

			if (fabs(xDiff) > 64 || fabs(yDiff) > 64)
				return false;

			if (fabs(xDiff) > fabs(yDiff))
			{
				if (xDiff < 0)
				{
					setPosition(p_oObject->getPosition().x + 64, getPosition().y);
				}
				else
				{
					setPosition(p_oObject->getPosition().x - 64, getPosition().y);
				}
			}
			else
			{
				if (yDiff < 0)
				{
					setPosition(getPosition().x, p_oObject->getPosition().y + 64);
				}
				else
				{
					setPosition(getPosition().x, p_oObject->getPosition().y - 64);
				}
			}
		}
		else if (p_oObject->getType() == DOOR)
		{
			Door *doorObj = dynamic_cast<Door*>(p_oObject);

			if (doorObj->getIsRemoved() == true)
				return false;

			if (doorObj->IsOpen() == true)
				return false;

			if (doorObj->IsVertical() == true)
			{
				float xDiff = p_oObject->getPosition().x - getPosition().x;
				float yDiff = p_oObject->getPosition().y - getPosition().y;

				if (fabs(xDiff) > 64 || fabs(yDiff) > 64)
					return false;

				if (fabs(xDiff) > fabs(yDiff))
				{
					if (xDiff < 0)
					{
						setPosition(p_oObject->getPosition().x + 10, getPosition().y);
					}
					else
					{
						setPosition(p_oObject->getPosition().x - 64, getPosition().y);
					}
					
				}
				else
				{
					if (yDiff < 0)
					{
						setPosition(getPosition().x, p_oObject->getPosition().y + 64);
					}
					else
					{
						setPosition(getPosition().x, p_oObject->getPosition().y - 64);
					}
				}
			}
			else
			{
				float xDiff = p_oObject->getPosition().x - getPosition().x;
				float yDiff = p_oObject->getPosition().y - getPosition().y;

				if (fabs(xDiff) > 64 || fabs(yDiff) > 64)
					return false;

				if (fabs(xDiff) > fabs(yDiff))
				{
					if (xDiff < 0)
					{
						setPosition(p_oObject->getPosition().x, getPosition().y + 64);
					}
					else
					{
						setPosition(p_oObject->getPosition().x, getPosition().y - 64);
					}
				}
				else
				{
					if (yDiff < 0)
					{
						setPosition(getPosition().x, p_oObject->getPosition().y + 31);
					}
					else
					{
						setPosition(getPosition().x, p_oObject->getPosition().y - 45);
					}
				}
			}

			
		}
		else if (p_oObject->getType() == PATROLLINGGUARD || p_oObject->getType() == STATIONARYGUARD)
		{
			if (p_oObject->getIsRemoved())
				return false;

			float xDiff = p_oObject->getPosition().x - getPosition().x;
			float yDiff = p_oObject->getPosition().y - getPosition().y;

			if (fabs(xDiff) > 25 || fabs(yDiff) > 25)
				return false;

			reset();
			m_xLevel->reset();
		}
		else if (p_oObject->getType() == EXIT)
		{
			reset();
			m_xLevel->reset();
		}

		return false;
	}

	bool PlayerObject::getHasCard()
	{
		return m_bHasCard;
	}

	bool PlayerObject::getHasObj()
	{
		return m_bHasObj;
	}

	int PlayerObject::getCurWep()
	{
		return m_iCurWep;
	}

	sf::CircleShape* PlayerObject::getattack()
	{
		if (m_rAttack != nullptr)
			return m_rAttack;
	}

	sf::Vector2f PlayerObject::getRangeToObject(GameObject *obj)
	{
		sf::Vector2f length;

		length.x = fabs(obj->getPosition().x - getPosition().x);
		length.y = fabs(obj->getPosition().y - getPosition().y);

		return sf::Vector2f();
	}

	void PlayerObject::setStateObjects(std::vector<GameObject*> &p_vStateObjects)
	{
		m_vStateObjects = &p_vStateObjects;
	}
}
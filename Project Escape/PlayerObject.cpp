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
#include "SoundRipple.h"
#include "SoundManager.h"
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
		m_walkspeed = 5.0f;
		m_sneakspeed = 2.0f;
		m_fTimehidden = 0.0f;
		m_fTimeUnhidden = 0.0f;

		m_sStepMusic = nullptr;

		m_iCurWep = 0;
		m_bHasCard = true;
		m_bTutComplete = false;

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

		m_vRemoveObjects = new std::vector<GameObject*>();

		m_xAnimator = p_xAnimator;

		m_xAnimator->loadAnimations("playerAnims.txt");

		std::vector<int> triggers = {
			3, 9
		};

		m_xAnimator->getAnimation("Spy_walk.txt")->setTriggers(triggers);

		triggers.clear();

		triggers.push_back(4);

		m_xAnimator->getAnimation("Spy_shot.txt")->setTriggers(triggers);

		m_xAnimator->getAnimation("Spy_baton.txt")->setTriggers(triggers);
	}

	void PlayerObject::update(float deltaTime, std::vector<GameObject*> objects)
	{

		bool bIsWalking = false;

		if (m_xWeapon->getAttachedObject() == nullptr)
			m_xWeapon->setAttachedObject(this);

		if (m_xWeapon != nullptr)
			m_xWeapon->update(deltaTime);

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			if (m_xWeapon->getCurrentWeaponType() == WEAPONGUN)
			{
				m_xAnimator->setForcedAnimation("Spy_shot.txt");
			}
			else if (m_xWeapon->getCurrentWeaponType() == WEAPONGARROTE)
			{
				Attack();
			}
			else if (m_xWeapon->getCurrentWeaponType() == WEAPONBATON)
			{
				m_xAnimator->setForcedAnimation("Spy_baton.txt");
			}
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
						if (object->getIsRemoved())
						{
							m_vRemoveObjects->push_back(object);
						}
					}
				}
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
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
					bIsWalking = true;
				}
				else
				{
					move(0, -m_walkspeed);
					m_xAnimator->setCurrentAnimation("Spy_walk.txt");
					bIsWalking = true;
				}
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && !m_bDblock)
			{
				if (m_sneaking)
				{
					move(0, m_sneakspeed);
					m_xAnimator->setCurrentAnimation("Spy_sneak.txt");
					bIsWalking = true;
				}

				else
				{
					bIsWalking = true;
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
					bIsWalking = true;
				}
				else
				{
					bIsWalking = true;
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
					bIsWalking = true;
				}
				else
				{
					bIsWalking = true;
					move(m_walkspeed, 0);
					m_xAnimator->setCurrentAnimation("Spy_walk.txt");
				}
			}
			if (!m_bAngleLocked)
				setRotation(calcAngle(sf::Mouse::getPosition(*p_window).x + getPosition().x - p_window->getSize().x * 0.5, sf::Mouse::getPosition(*p_window).y + getPosition().y - p_window->getSize().y * 0.5));
		}

		if (m_hiding == true)
		{
			m_fTimeUnhidden = 0.0f;
			m_fTimehidden += deltaTime;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) && m_fTimehidden > 2)
			{
				m_hiding = false;
				move(0, 0);
				m_fTimehidden = 0.0f;
			}
		}
		else
			m_fTimeUnhidden += deltaTime;


		m_bRblock = false;
		m_bLblock = false;
		m_bUblock = false;
		m_bDblock = false;

		if (m_xWeapon != nullptr)
			m_xWeapon->update(deltaTime);

		if (!m_hiding)
			m_xAnimator->update(deltaTime);

		if (!bIsWalking && !m_sneaking)
		{
			m_xAnimator->setCurrentAnimation("Spy_idle.txt");
		}
		else if (!bIsWalking && m_sneaking)
		{
			m_xAnimator->setCurrentAnimation("Spy_idle.txt");
		}

		static_cast<AnimatedSprite*>(m_xSprite)->update(deltaTime);
		m_xAnimator->update(deltaTime);

		if (m_xAnimator->getCurrentAnimationID().compare("Spy_shot.txt") == 0)
		{
			if (m_xAnimator->getCurrentAnimation()->getIsTrigger())
			{
				if (m_xWeapon->m_iCurrentCharges > 0)
				{
					sf::Vector2f ripplePosition = getPosition();
					ripplePosition.x -= cosf((getRotation() - 180) * 0.0174532925) * 60;
					ripplePosition.y -= sinf((getRotation() - 180) * 0.0174532925) * 60;
					SoundRipple *ripple = new SoundRipple(ripplePosition, 8, 700, 0.7f);
					m_vStateObjects->insert(m_vStateObjects->begin(), ripple);
					Attack();
				}
				else
				{
					delete m_xWeapon;
					m_xWeapon = nullptr;
					m_xWeapon = new Garrote(1.f, 2.5f, m_vStateObjects);
					m_xWeapon->setAttachedObject(this);
				}
			}
		}

		if (m_xAnimator->getCurrentAnimationID().compare("Spy_baton.txt") == 0)
		{
			if (m_xAnimator->getCurrentAnimation()->getIsTrigger())
			{
				Attack();
			}
		}

		if (m_xAnimator->getCurrentAnimationID().compare("Spy_walk.txt") == 0)
		{
			if (m_xAnimator->getCurrentAnimation()->getIsTrigger())
			{

				sf::Vector2f ripplePosition = getPosition();



				ripplePosition.x -= cosf((getRotation() - 180) * 0.0174532925) * 16;
				ripplePosition.y -= sinf((getRotation() - 180) * 0.0174532925) * 16;

				SoundRipple *ripple = new SoundRipple(ripplePosition, 8, 150, 1.f);
				m_vStateObjects->insert(m_vStateObjects->begin(), ripple);



				if (m_sStepMusic == nullptr)
				{
					SoundManager soundmanager("../resources/Music/");
					m_sStepMusic = soundmanager.getMusic("steps.wav");
					m_sStepMusic->setVolume(100.0f);
					m_sStepMusic->setLoop(false);

				}

				m_sStepMusic->play();


			}
			m_vRemoveObjects->clear();
		}
		if (m_bTutComplete != true)
		{
			if (getPosition().x < 49 * 64)
				m_xLevel->handleTutObjects(1, true);

			if (getPosition().y > 9 * 64)
			{
				m_xLevel->handleTutObjects(1, false);
				if (getPosition().x > 49 * 64 && getPosition().y < 64 * 11)
					m_xLevel->handleTutObjects(2, true);
			}

			if (getPosition().x > 52 * 64)
				m_xLevel->handleTutObjects(2, false);

			if (getPosition().x > 57 * 64 && getPosition().y > 11 * 64)
				m_xLevel->handleTutObjects(3, true);
			else
				m_xLevel->handleTutObjects(3, false);

			if (getPosition().x > 60 * 64 || getPosition().y > 14 * 64)
				m_xLevel->handleTutObjects(3, false);

			if (getPosition().y > 11 * 64 && getPosition().x < 56 * 64)
				m_xLevel->handleTutObjects(4, true);
			else
				m_xLevel->handleTutObjects(4, false);

			if (getPosition().x > 59 * 64)
			{
				m_xLevel->handleTutObjects(5, true);
				if (getPosition().y > 12 * 64 && m_hiding == true)
				{
					//m_xLevel->handleTutObjects(5, false);
					//m_xLevel->handleTutObjects(6, true);
				}
				if (getPosition().y > 17)
				{
					//m_xLevel->handleTutObjects(6. false);
				}
			}
			if (getPosition().y > 19 * 64)
				m_bTutComplete = true;

		}
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
		m_xWeapon = new Garrote(1.f, 2.5f, m_xLevel->getObjects());
		m_iCurWep = 0;
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

		states.transform *= getTransform();
		target.draw(*static_cast<AnimatedSprite*>(m_xAnimator->getCurrentAnimation()), states);
	}

	void PlayerObject::setInteractionRange(float range)
	{
		m_interactionRange = range;
	}

	void PlayerObject::Interract(GameObject *p_xInteractObj)
	{
		if (p_xInteractObj->getType() == LOCKERL ||
			p_xInteractObj->getType() == LOCKERR)
		{
			if (m_fTimeUnhidden > 1.0f)
			{
				setPosition(p_xInteractObj->getPosition().x, p_xInteractObj->getPosition().y);
				m_hiding = true;
				setRotation(0.0f);
			}
		}
		else if (p_xInteractObj->getType() == LOCKERD ||
			p_xInteractObj->getType() == LOCKERU)
		{
			if (m_fTimeUnhidden > 1.0f)
			{
				setPosition(p_xInteractObj->getPosition().x, p_xInteractObj->getPosition().y);
				m_hiding = true;
				setRotation(90.0f);
			}

		}
		else if (p_xInteractObj->getType() == BATON)
		{
			if (p_xInteractObj->getIsRemoved() != false)
				return;

			Item *baton = static_cast<Item*>(p_xInteractObj);
			m_xWeapon = nullptr;
			m_xWeapon = new Baton(true, baton->getCharges(), 1.f, 1.f, m_vStateObjects, m_xLevel, m_xGobjManager, m_xLevel->getSpriteManager());
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

			Item *gun = static_cast<Item*>(p_xInteractObj);
			m_xWeapon = new Gun(true, gun->getCharges(), 1.f, 1.f, m_vStateObjects, m_xGobjManager, m_xLevel->getSpriteManager());
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
		if (m_xWeapon->getCurrentWeaponType() == EWeaponType::WEAPONGARROTE)
		{
			Garrote *garrote = static_cast<Garrote*>(m_xWeapon);
			garrote->trigger();
		}
		else if (m_xWeapon->getCurrentWeaponType() == EWeaponType::WEAPONBATON)
		{
			Baton *baton = static_cast<Baton*>(m_xWeapon);
			baton->attack();
		}
		else if (m_xWeapon->getCurrentWeaponType() == EWeaponType::WEAPONGUN)
		{

			Gun *gun = static_cast<Gun*>(m_xWeapon);
			gun->setTarget(sf::Vector2f(sf::Mouse::getPosition(*p_window)) + getPosition() - sf::Vector2f(960, 540));
			gun->attack();
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
		if (p_oObject->getType() == WALL || p_oObject->getType() == COUCH || p_oObject->getType() == PLANTS || p_oObject->getType() == DESKS)
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
			Door *doorObj = static_cast<Door*>(p_oObject);

			if (doorObj->getIsRemoved() == true)
				return false;

			if (doorObj->IsLocked() == 0)
			{
				if (doorObj->getIsClosing())
				{
					doorObj->Open();
				}
			}

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
		else if (p_oObject->getType() == PATROLLINGGUARD || p_oObject->getType() == STATIONARYGUARD)
		{
			if (p_oObject->getIsRemoved())
				return false;

			float xDiff = p_oObject->getPosition().x - getPosition().x;
			float yDiff = p_oObject->getPosition().y - getPosition().y;

			if (fabs(xDiff) > 25 || fabs(yDiff) > 25)
				return false;


			m_xLevel->setStateNum(2);
		}
		else if (p_oObject->getType() == EXIT)
		{
			m_xLevel->setStateNum(3);
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

	void PlayerObject::setStateObjects(std::vector<GameObject*> *p_vStateObjects)
	{
		m_vStateObjects = p_vStateObjects;
	}
}
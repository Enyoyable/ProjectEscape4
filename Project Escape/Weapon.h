#pragma once

#include "stdafx.h"
#include "Item.h"

namespace esc
{

	class GameObject;

	enum EWeaponType
	{
		WEAPONGARROTE,
		WEAPONBATON,
		WEAPONGUN
	};

	class Weapon
	{
	public:
		Weapon(EWeaponType p_eWeaponType, bool p_bHasCharges, int p_iAmountOfCharges, float p_fDelay, float p_fDuration, std::vector<GameObject*> *p_vObjects);

		virtual void update(float fDeltaTime);

		virtual void trigger();

		virtual void attack();

		virtual void Throw();

		virtual void drop();

		virtual void setTarget(sf::Vector2f p_v2fTarget);

		EWeaponType getCurrentWeaponType();

		void setAttachedObject(GameObject *p_xObject);

		GameObject *getAttachedObject();

	protected:
		EWeaponType m_eCurrentWeaponType;

		bool m_bIsFiring;

		bool m_bCanFire;

		float m_fAttackDuration;

		float m_fAttackDelay;

		float m_fAttackTimer;

		bool m_bHasFired;

		bool m_bHasCharges;

		int m_iCurrentCharges;

		int m_iMaxCharges;

		std::vector<GameObject*> *m_vObjects;

		float m_fAngle;

		GameObject *m_xAttachedObject;
	};

}
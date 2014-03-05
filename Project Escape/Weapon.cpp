#include "Weapon.h"

namespace esc
{
	Weapon::Weapon(EWeaponType p_eWeaponType, bool p_bHasCharges, int p_iAmountOfCharges, float p_fDelay, float p_fDuration, std::vector<GameObject*> *p_vObjects)
	{
		m_eCurrentWeaponType = p_eWeaponType;
		m_bHasCharges = p_bHasCharges;
		m_iMaxCharges = p_iAmountOfCharges;
		m_iCurrentCharges = p_iAmountOfCharges;

		m_bIsFiring = false;

		m_bCanFire = true;
		m_fAttackDuration = p_fDuration;
		m_fAttackDelay = p_fDelay;

		m_bHasFired = false;

		m_vObjects = p_vObjects;
	}

	void Weapon::update(float fDeltaTime)
	{
		//Do nothing by default
	}

	void Weapon::attack()
	{

	}

	void Weapon::trigger()
	{
		//Do nothing by default
	}

	EWeaponType Weapon::getCurrentWeaponType()
	{
		return m_eCurrentWeaponType;
	}

	void Weapon::setAttachedObject(GameObject *p_xObject)
	{
		m_xAttachedObject = p_xObject;
	}

	GameObject *Weapon::getAttachedObject()
	{
		return m_xAttachedObject;
	}
}
#pragma once

#include "stdafx.h"
#include "GameObject.h"

namespace esc 
{
	class DrawManager;
	class Level;

	class Item : public GameObject
	{
	public:
		Item(sf::Vector2f p_v2Pos, sf::Sprite* p_sSprite, bool p_bIsFlying , sf::Vector2f p_v2Dir, int p_iObjectId, EObjectType p_eType);

		virtual void update(float p_fDeltaTime);
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		void HandleCollision(GameObject *p_xGameObject, std::vector<GameObject*> *p_vRemoveVector);

		void setDir(sf::Vector2f p_v2fInpDir);
		sf::Vector2f getDir();

		void setIsFlying(bool p_bflying);
		bool getIsflying();

		void setTarget(sf::Vector2f p_v2fTarget);

	private:

		sf::Vector2f m_v2fTarget;

		bool m_bIsFlying;	
	};
}
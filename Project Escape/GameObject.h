#pragma once

#include "stdafx.h"

namespace esc
{
	
	class Corner;
	class DrawManager;
	class AnimatedSprite;

	enum EObjectType
	{
		WALL,
		BACKGROUNDOBJ,
		LOCKERD,
		LOCKERU,
		LOCKERR,
		LOCKERL,
		LOCKER,
		PLAYER,
		STATIONARYGUARD,
		PATROLLINGGUARD,
		TOILET,
		URINAL,
		URINAL2,
		SINK,
		EXIT,
		BUTTON,
		DESKS,
		COUCH,
		BATON,
		GUN,
		RADIO,
		KEYCARD,
		PDA,
		LOCKEDH,
		LOCKEDV,
		OPENH,
		OPENV,
		CLOSEDH,
		CLOSEDV,
		BULLET,
		SOUNDRIPPLE,
		WRITER,
		PLANTS,
		DOOR,
		RECMID,
		RECTOP,
		RECLOW,
		FIKABORD,
		STOLARUPP,
		STOLARNER,
		STOLARV,
		STOLARH,
		OBJECTIVE,
		TUTOBJ
	};

	class GameObject : public sf::Drawable, public sf::Transformable
	{
	public:
		GameObject(sf::Vector2f p_v2fPosition, sf::Vector2f p_v2fSize, bool p_bInteractable, int p_iObjectId, EObjectType p_eType, sf::Sprite *p_xSprite = nullptr);

		virtual void update(float p_fDeltaTime);

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		bool isHideable();

		void setSprite(sf::Sprite *p_xSprite);
		sf::Sprite *getSprite();

		void setSize(sf::Vector2f p_v2fSize);
		sf::Vector2f getSize();

		void setInteractable(bool p_bInteractable);
		bool getInteractable();

		void setTransparent(bool p_bIsTransparent);
		bool getTransparent();

		void createCorners();
		std::vector<Corner*> getCorners();

		void setId(int p_iObjectId);
		int getId();

		void setIsRemoved(bool p_bIsRemoved);
		bool getIsRemoved();

		void setType(EObjectType p_eObjectType);
		EObjectType getType();

		void HandleCollision(GameObject *p_oGameObject);

		bool m_bAngleLocked;

		void setAnimation(AnimatedSprite *p_xAnimatedSprite);

	protected:
		sf::Sprite *m_xSprite;

		std::vector<Corner*> m_vCorners;

		bool m_bInteractable;
		bool m_bIsTransparent;
		bool  m_bIsRemoved;

		int m_iObjectId;

		sf::Vector2f m_v2fSize;

		EObjectType m_eObjectType;

		AnimatedSprite *m_xAnimatedSprite;

		bool m_bHasAnimation;
	};
}


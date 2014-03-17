#pragma once

#include "stdafx.h"

#include "GameObject.h"

namespace esc
{

	class GameObject;

	class SoundRipple : public GameObject
	{

	public:
		SoundRipple(sf::Vector2f p_v2fPosiiton, float p_fMinRadious, float p_fmaxRadious, float p_fDuration);

		virtual void update(float p_fDeltaTime);

		void destroy();

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		virtual void HandleCollision(GameObject *p_xGameObject);

		bool m_bIsUpdating;

	private:
		float m_fCurrentRadious;

		float m_fMaxRadious;

		float m_fMinRadious;

		float m_fCurrentDuration;

		float m_fMaxDuration;

		sf::Vector2f m_v2fOriginPosition;

		sf::Vector2f *m_v2fCurrentPos;
	};

}
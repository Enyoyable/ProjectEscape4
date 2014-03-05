#pragma once

#include "stdafx.h"

namespace esc
{

	class GameObject;
	class Corner;

	class LightSource : public sf::Transformable, public sf::Drawable
	{

	private:
		struct SDeadSector
		{
			float m_fStartingAngle;
			float m_fFinishAngle;
		};

	public:
		LightSource(sf::Vector2f p_v2fPosition, float p_fRadious, std::vector<GameObject*> *p_vObjects, float p_fAngleWidth);
		LightSource(sf::Vector2f p_v2fPosition, sf::Vector2f p_v2fSize, float p_fRadious, std::vector<GameObject*> *p_vObjects, float p_fAngleWidth);

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		void setRadious(float p_fRadious);
		float getRadious();

		void setDirection(sf::Vector2f p_v2fDirection);
		float getDirectionAngle();

		void update(float p_fDeltaTime);

		void createLight();

		void getRelevantCorners(std::vector<Corner*> *p_vCorners, std::map<Corner*, float> *p_mCornerAngles);

		void createFan(std::vector<Corner*> *p_vCorners, std::map<Corner*, float> *p_mCornerAngles);

		float getCornerAngle(Corner* p_xCorner);

	private:
		float m_fRadious;

		float m_fMiddleAngle;

		float m_fStartAngle;

		float m_fLightWidthAngle;

		bool m_bIsDot;

		std::vector<GameObject*> *m_vObjects;

		sf::Vector2f m_v2fSize;

		std::vector<Corner*> m_vCorners;

		std::vector<sf::Vertex*> m_vTriangleFan;
	};

}
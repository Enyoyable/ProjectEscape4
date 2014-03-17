#pragma once

#include "stdafx.h"

#include "GameObject.h"

namespace esc
{
	
	class PlayerObject;
	class AIManager;
	class GameObjectManager;
	class Level;
	class PathFind;

	class Guard : public GameObject
	{

		friend class Baton;
		friend class Bullet;
		friend class Garrote;
		friend class AIStateChasing;

	public:
		Guard(sf::Vector2f p_v2fPosition, sf::Vector2f p_v2fSize, bool p_bInteractable, int p_iObjectId, PlayerObject *p_xPlayer, GameObjectManager *p_xGameObjectManager, Level *level, PathFind *p_xPathFind, sf::Sprite *p_xSprite = nullptr);

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		virtual void update(float p_fDeltaTime);

		bool searchForPlayer(PlayerObject *p_xObject);

		void setDirection(sf::Vector2f p_v2fDirection);

		void setWatchSize(float p_fAngle);

		void setVisionRange(float p_fRange);

		void setRotationPoints(float p_fMaxAngle, float p_fMinAngle, bool p_bClockwise);
		void setConstantRotation(bool p_fClockwise);
		bool getIsRotating();

		void setAngle(float p_fAngle);

		void reset();

		void addPatrolPoints(sf::Vector2f *p_v2fPatrolPoint);

		void setPatrolPath(std::vector<sf::Vector2f*> p_vPatrolPath);

		void removePatrolPoint(int m_iIndex);

		void clearPatrolPoints();

		void isHit();

		std::vector<sf::Vector2f*> getPatrolPoints();

		void setIsPatrolling(bool p_bIsPatrolling);

		bool getIsPatrolling();

		void attachAi(AIManager *p_xAIManager);

		void addDrawPath(std::vector<sf::Vector2f*> p_vPath);

		void alert(sf::Vector2f p_v2fPosition);

		void updateRotation(float p_fDeltaTime);

		void updatePatrolling(float p_fDeltaTime);

		sf::Vector2f *getCurrentPatrolPoint();

		bool followPath(float p_fDeltaTime);

		void setFollowPath(std::vector<sf::Vector2f*> p_v2fFollowPath);

		AIManager *m_xAIManager;

		int followPathCount = 0;

		sf::Vector2f m_v2fStartPosition;

	private:
		bool m_bIsPatrolling;

		bool m_bIsChasing;

		bool m_bIsRotatingClockwise;

		bool m_bHasPassedZero;

		bool m_bHasConstantRotation;
		
		float m_fWatchAngle;
		float m_fWatchArea;

		float m_fMaxRotationAngle;
		float m_fMinRotationAngle;

		bool m_bIsRotating;

		float m_fStartAngle;

		

		float m_fViewDistance;

		PlayerObject *m_xPlayerObject;

		std::vector<sf::Vector2f*> m_vPatrolPoints;

		int m_iPatrolPointCount;

		int m_iCurrentPatrolPointIndex;

		float m_fGuardPatrolSpeed;

		float m_fGuardChaseSpeed;

		bool m_isDead;

		bool m_bHasAI;

		std::vector<sf::Vector2f*> m_vFollowPath;

		GameObjectManager *m_xGobjManager;

		Level *m_xLevel;

		std::vector<sf::Vector2f*> m_vPath;

		
	};

}
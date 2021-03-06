#pragma once

#include "GameObject.h"

#include "stdafx.h"

namespace esc
{

	class SpriteManager;
	class GameObjectManager;
	class PathNode;
	class PathFind;

	class Level : public sf::Drawable
	{

		enum EFloorType
		{
			NORMAL,
			NORMALFLOOR,
			FLOOR2,
			FLOOR3,
			FLOOR4
		};

		struct SGuardRotation
		{
			float m_fMaxAngle, m_fMinAngle, m_fSize, m_fRange;
			bool m_bConstant, m_bClockwise;
		};

	public:
		Level(std::string p_sDirectoryPath, SpriteManager *p_xSpriteManager, GameObjectManager *p_xGameObjectManager);

		virtual void update(float p_fDeltaTime);

		void loadColorCodes(std::string p_sFilePath);

		void loadFloorColorCodes(std::string p_sFilePath);

		void create(std::string p_sFilePath, std::string p_sPatrolPath);

		void createFloor(std::string p_sFilePath);

		void getPatrolPaths(std::string p_sFilePath);

		void getRotationsPaths(std::string p_sFilePath);

		void reset();

		void destroy();

		void handleTutObjects(int p_iObjNum, bool p_bAddOrRemove);

		std::vector<std::vector<PathNode*>> *getPathNodes();

		std::vector<GameObject*> *getObjects();

		std::vector<GameObject*> *getTutObjects();

		SpriteManager* getSpriteManager();

		void setStateNum(int p_statenum);
		int getStateNum();

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		void getWalls(GameObject *p_aWalls[200][200]);

	private:
		std::map<sf::Color*, EObjectType> m_mColorCodes;

		std::vector<sf::Color*> m_vColorCodes;

		std::map<sf::Color*, EFloorType> m_mFloorColorCodes;

		std::vector<sf::Color*> m_vFloorColorCodes;

		std::vector<sf::Sprite*> m_vFloorSprites;

		std::vector<GameObject*> m_vLevelObjects;

		std::vector<GameObject*> m_vTutorialObjects;

		std::string m_sDirectoryPath;

		SpriteManager *m_xSpriteManager;

		GameObjectManager *m_xGameObjectManager;

		int m_iGuardCount;

		int m_iRotateCount;

		std::vector<std::vector<sf::Vector2f*>> m_vPatrolPaths;

		std::vector<SGuardRotation*> m_vGuardRotations;

		GameObject *m_aAdjacantObjects[200][200];
		GameObject *m_aWalls[200][200];

		int m_ixDimensions;
		int m_iyDimensions;
		int m_iStateNum;

		std::vector<std::vector<PathNode*>> m_vPathNodes;

		PathFind *m_xPathFinder;
	};

}
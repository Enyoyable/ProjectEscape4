#pragma once

#include "stdafx.h"
#include "PathNode.h"

namespace esc
{

	class GameObject;
	class Level;

	class PathFind
	{
	public:
		PathFind(Level *p_xLevel);

		std::vector<sf::Vector2f*> pathToPosition(GameObject *p_xPathingObject, sf::Vector2f *p_v2fTargetPosition);

		std::vector<sf::Vector2f*> pathToPosition(sf::Vector2f *p_v2fStartPosition, sf::Vector2f *p_v2fTargetPosition);
		
		std::vector<sf::Vector2f*> pathToObject(GameObject *p_xPathingObject, GameObject *p_xTargetObject);

		void getAdjacentNodes();

		void calculateNode(PathNode *p_xNode, bool p_bDiagonal);

		void getPath();

	private:
		Level *m_xLevel;

		GameObject *m_xPathingObject;
		GameObject *m_xTargetObject;

		PathNode *m_xTargetNode;
		PathNode *m_xCurrentNode;
		PathNode *m_xStartingNode;

		std::vector<PathNode*> m_vClosedList;
		std::vector<PathNode*> m_vOpenList;

		std::vector<std::vector<PathNode*>> m_vPathNodes;

		std::vector<PathNode*> m_vAdjacentNodes;

		std::vector<sf::Vector2f*> m_vPathPoints;
	};
}
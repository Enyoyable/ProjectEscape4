#include "PathFind.h"
#include "Level.h"

namespace esc
{
	PathFind::PathFind(Level *p_xLevel)
	{
		m_vPathNodes = *p_xLevel->getPathNodes();

		m_xLevel = p_xLevel;

		m_xPathingObject = nullptr;
		m_xTargetObject = nullptr;
		m_xTargetNode = nullptr;
		m_xCurrentNode = nullptr;
		m_xStartingNode = nullptr;
	}

	std::vector<sf::Vector2f*> PathFind::pathToPosition(GameObject *p_xPathingObject, sf::Vector2f *p_v2fTargetPosition)
	{
		for (auto node : m_vOpenList)
		{
			node->F = 0;
			node->G = 0;
			node->H = 0;

			node->setOpen(false);
			node->setParent(nullptr);
		}

		for (auto node : m_vClosedList)
		{
			node->F = 0;
			node->G = 0;
			node->H = 0;

			node->setClosed(false);
			node->setParent(nullptr);
		}

		m_vOpenList.clear();
		m_vClosedList.clear();
		m_vPathPoints.clear();

		sf::Vector2i v2fTargetNode;

		v2fTargetNode.x = (int)p_v2fTargetPosition->x / 64;
		v2fTargetNode.y = (int)p_v2fTargetPosition->y / 64;

		m_xTargetNode = m_vPathNodes[v2fTargetNode.y][v2fTargetNode.x];

		sf::Vector2i v2fCurrentNode;

		v2fCurrentNode.x = (int)p_xPathingObject->getPosition().x / 64;
		v2fCurrentNode.y = (int)p_xPathingObject->getPosition().y / 64;

		m_xCurrentNode = m_vPathNodes[v2fCurrentNode.y][v2fCurrentNode.x];
		m_xStartingNode = m_xCurrentNode;

		getPath();

		return m_vPathPoints;
	}

	std::vector<sf::Vector2f*> PathFind::pathToPosition(sf::Vector2f *p_v2fStartPosition, sf::Vector2f *p_v2fTargetPosition)
	{
		for (auto node : m_vOpenList)
		{
			node->F = 0;
			node->G = 0;
			node->H = 0;

			node->setOpen(false);
			node->setParent(nullptr);
		}

		for (auto node : m_vClosedList)
		{
			node->F = 0;
			node->G = 0;
			node->H = 0;

			node->setClosed(false);
			node->setParent(nullptr);
		}

		m_vOpenList.clear();
		m_vClosedList.clear();
		m_vPathPoints.clear();

		sf::Vector2i v2fTargetNode;

		v2fTargetNode.x = (int)p_v2fTargetPosition->x / 64;
		v2fTargetNode.y = (int)p_v2fTargetPosition->y / 64;

		m_xTargetNode = m_vPathNodes[v2fTargetNode.y][v2fTargetNode.x];

		sf::Vector2i v2fCurrentNode;

		v2fCurrentNode.x = (int)p_v2fStartPosition->x / 64;
		v2fCurrentNode.y = (int)p_v2fStartPosition->y / 64;

		m_xCurrentNode = m_vPathNodes[v2fCurrentNode.y][v2fCurrentNode.x];
		m_xStartingNode = m_xCurrentNode;

		getPath();

		return m_vPathPoints;
	}

	std::vector<sf::Vector2f*> PathFind::pathToObject(GameObject *p_xPathingObject, GameObject *p_xTargetObject)
	{

		m_vOpenList.clear();
		m_vClosedList.clear();
		m_vPathPoints.clear();

		sf::Vector2i v2fTargetNode, v2fCurrentNode;

		v2fTargetNode.x = (int)(p_xTargetObject->getPosition().x) / 64;
		v2fTargetNode.y = (int)(p_xTargetObject->getPosition().y) / 64;

		/*if (v2fTargetNode.x > 199)
			v2fTargetNode.x = 199;
		else if (v2fTargetNode.x < 0)
			v2fTargetNode.x = 0;

		if (v2fTargetNode.y > 199)
			v2fTargetNode.y = 199;
		else if (v2fTargetNode.y < 0)
			v2fTargetNode.y = 0;*/

		m_xTargetNode = m_vPathNodes[v2fTargetNode.y][v2fTargetNode.x];

		v2fCurrentNode.x = (int)(p_xPathingObject->getPosition().x) / 64;
		v2fCurrentNode.y = (int)(p_xPathingObject->getPosition().y) / 64;

		m_xCurrentNode = m_vPathNodes[v2fCurrentNode.y][v2fCurrentNode.x];
		m_xStartingNode = m_xCurrentNode;

		getPath();

		for (auto node : m_vOpenList)
		{
			node->F = 0;
			node->G = 0;
			node->H = 0;

			node->setOpen(false);
			node->setParent(nullptr);
		}

		for (auto node : m_vClosedList)
		{
			node->F = 0;
			node->G = 0;
			node->H = 0;

			node->setClosed(false);
			node->setParent(nullptr);
		}

		return m_vPathPoints;
	}

	void PathFind::getAdjacentNodes()
	{
		m_vAdjacentNodes.clear();

		sf::Vector2i currentNodePosition = m_xCurrentNode->getPosition();
		
		PathNode *xUpperNode = nullptr;
	
		PathNode *xLowerNode = nullptr;

		PathNode *xRightNode = nullptr;

		PathNode *xLeftNode = nullptr;

		if (currentNodePosition.y - 1 >= 0)
			xUpperNode = m_vPathNodes[currentNodePosition.y - 1][currentNodePosition.x];
		
		if (currentNodePosition.y + 1 < m_vPathNodes.size())
			xLowerNode = m_vPathNodes[currentNodePosition.y + 1][currentNodePosition.x];

		if (currentNodePosition.x + 1 < m_vPathNodes[currentNodePosition.y].size())
			xRightNode = m_vPathNodes[currentNodePosition.y][currentNodePosition.x + 1];

		if (currentNodePosition.x - 1 >= 0)
			xLeftNode = m_vPathNodes[currentNodePosition.y][currentNodePosition.x - 1];

		bool bLeftFree = !(xLeftNode == nullptr || xLeftNode->getIllegal() || xLeftNode->getIsClosed());

		bool bRightFree = !(xRightNode == nullptr || xRightNode->getIllegal() || xRightNode->getIsClosed());

		bool bUpperFree = !(xUpperNode == nullptr || xUpperNode->getIllegal() || xUpperNode->getIsClosed());

		bool bLowerFree = !(xLowerNode == nullptr || xLowerNode->getIllegal() || xLowerNode->getIsClosed());

		if (bLeftFree)
		{
			m_vAdjacentNodes.push_back(xLeftNode);
			calculateNode(xLeftNode, false);
		}
			
		if (bRightFree)
		{
			m_vAdjacentNodes.push_back(xRightNode);
			calculateNode(xRightNode, false);
		}
			
		if (bUpperFree)
		{
			m_vAdjacentNodes.push_back(xUpperNode);
			calculateNode(xUpperNode, false);
		}
			
		if (bLowerFree)
		{
			m_vAdjacentNodes.push_back(xLowerNode);
			calculateNode(xLowerNode, false);
		}

		if (bLeftFree && bUpperFree)
		{
			m_vAdjacentNodes.push_back(m_vPathNodes[currentNodePosition.y - 1][currentNodePosition.x - 1]);
			calculateNode(m_vPathNodes[currentNodePosition.y - 1][currentNodePosition.x - 1], true);
		}

		if (bLeftFree && bLowerFree)
		{
			m_vAdjacentNodes.push_back(m_vPathNodes[currentNodePosition.y + 1][currentNodePosition.x - 1]);
			calculateNode(m_vPathNodes[currentNodePosition.y + 1][currentNodePosition.x - 1], true);
		}

		if (bRightFree && bUpperFree)
		{
			m_vAdjacentNodes.push_back(m_vPathNodes[currentNodePosition.y - 1][currentNodePosition.x + 1]);
			calculateNode(m_vPathNodes[currentNodePosition.y - 1][currentNodePosition.x + 1], true);
		}

		if (bRightFree && bLowerFree)
		{
			m_vAdjacentNodes.push_back(m_vPathNodes[currentNodePosition.y + 1][currentNodePosition.x + 1]);
			calculateNode(m_vPathNodes[currentNodePosition.y + 1][currentNodePosition.x + 1], true);
		}
	}

	void PathFind::calculateNode(PathNode *p_xNode, bool p_bDiagonal)
	{
		if (p_xNode->getIllegal())
			return;

		if (p_xNode->getIsClosed())
			return;

		int iAddedG;

		if (p_bDiagonal)
		{
			iAddedG = 14;
		}
		else
		{
			iAddedG = 10;
		}

		if (!p_xNode->getIsOpen())
		{
			p_xNode->G = iAddedG + m_xCurrentNode->G;

			sf::Vector2i v2iNodeCoords = p_xNode->getPosition();

			sf::Vector2i v2iDistance = v2iNodeCoords - m_xTargetNode->getPosition();

			abs(v2iDistance.x);
			abs(v2iDistance.y);

			p_xNode->H = abs(v2iDistance.x * 10) + abs(v2iDistance.y * 10);

			p_xNode->F = p_xNode->H + p_xNode->G;

			p_xNode->setOpen(true);
			p_xNode->setParent(m_xCurrentNode);
			m_vOpenList.push_back(p_xNode);
		}
		else
		{
			int G, H, F;

			G = p_xNode->G;
			F = p_xNode->F;
			H = p_xNode->H;

			if (m_xCurrentNode->G + iAddedG < G)
			{
				p_xNode->G = iAddedG + m_xCurrentNode->G;

				p_xNode->F = p_xNode->H + p_xNode->G;
				p_xNode->setParent(m_xCurrentNode);
			}
		}
	}

	void PathFind::getPath()
	{
		if (m_xCurrentNode == m_xTargetNode)
			return;

		/*if (m_xCurrentNode->getIllegal())
			m_xCurrentNode->setIllegal(false);*/

		if (m_xTargetNode->getIllegal())
			m_xTargetNode->setIllegal(false);

		bool bFinished = false;

		m_vClosedList.push_back(m_xCurrentNode);
		m_xCurrentNode->setClosed(true);

		for (auto iter = m_vOpenList.begin(); iter != m_vOpenList.end(); ++iter)
		{
			if (*iter == m_xCurrentNode)
			{
				//m_xCurrentNode->setOpen(false);
				m_vOpenList.erase(iter);
				break;
			}
		}

		getAdjacentNodes();

		while (!bFinished)
		{
			if (m_vOpenList.size() <= 0)
				return;

			PathNode *xNode = m_vOpenList[0];
			for (auto node : m_vOpenList)
			{
				if (node->F < xNode->F)
				{
					xNode = node;
				}
			}

			m_vClosedList.push_back(xNode);
			xNode->setClosed(true);

			for (auto iter = m_vOpenList.begin(); iter != m_vOpenList.end(); ++iter)
			{
				if (*iter == xNode)
				{
					//xNode->setOpen(false);
					m_vOpenList.erase(iter);
					break;
				}
			}

			m_xCurrentNode = xNode;

			if (m_xCurrentNode == m_xTargetNode)
			{
				bFinished = true;
				break;
			}

			getAdjacentNodes();
		}

		bFinished = false;

		PathNode *xParentNode = m_xTargetNode;

		int max = m_vClosedList.size();

		int i = 0;

		for (int y = 0; y < 150; y++)
		{
			for (int x = 0; x < 150; x++)
			{
				m_vPathNodes[y][x]->setOpen(false);
			}
		}

		while (xParentNode != m_xStartingNode && i++ < max)
		{
			m_vPathPoints.push_back(new sf::Vector2f(xParentNode->getPosition().x * 64, xParentNode->getPosition().y * 64));

			xParentNode = xParentNode->getParent();
		}
	}
}
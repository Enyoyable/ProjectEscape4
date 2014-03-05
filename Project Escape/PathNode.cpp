#include "PathNode.h"

namespace esc
{
	PathNode::PathNode(sf::Vector2i p_v2iPosition, bool p_bIsIllegal)
	{
		m_v2iPosition = p_v2iPosition;

		m_bIsClosed = false;
		m_bIsIllegal = p_bIsIllegal;

		G = 0;
		F = 0;
		H = 0;

		m_xParent = nullptr;
	}

	void PathNode::setClosed(bool p_bIsClosed)
	{
		m_bIsClosed = p_bIsClosed;
	}

	void PathNode::setOpen(bool p_bIsOpen)
	{
		m_bIsOpen = p_bIsOpen;
	}

	void PathNode::setIllegal(bool p_bIsIllegal)
	{
		m_bIsIllegal = p_bIsIllegal;
	}

	void PathNode::setParent(PathNode *p_xParent)
	{
		m_xParent = p_xParent;
	}

	PathNode *PathNode::getParent()
	{
		return m_xParent;
	}

	void PathNode::setPosition(sf::Vector2i p_v2iPosition)
	{
		m_v2iPosition = p_v2iPosition;
	}

	sf::Vector2i PathNode::getPosition()
	{
		return m_v2iPosition;
	}

	bool PathNode::getIllegal()
	{
		return m_bIsIllegal;
	}

	bool PathNode::getIsClosed()
	{
		return m_bIsClosed;
	}

	bool PathNode::getIsOpen()
	{
		return m_bIsOpen;
	}
}
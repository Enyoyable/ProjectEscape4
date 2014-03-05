#pragma once

#include "stdafx.h"

namespace esc
{
	class PathNode
	{
	public:
		PathNode(sf::Vector2i p_v2iPosition, bool p_bIsIllegal);

		void setIllegal(bool p_bIsIllegal);

		void setClosed(bool p_bIsclosed);

		void setOpen(bool p_bIsOpen);

		void setParent(PathNode *p_xParent);

		void setPosition(sf::Vector2i p_v2iPosition);

		bool getIllegal();

		bool getIsClosed();

		bool getIsOpen();

		sf::Vector2i getPosition();

		PathNode *getParent();

		int F, G, H;

	private:

		bool m_bIsOpen;
		bool m_bIsClosed;
		bool m_bIsIllegal;

		PathNode *m_xParent;

		sf::Vector2i m_v2iPosition;
	};
}
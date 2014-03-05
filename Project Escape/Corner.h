#pragma once

#include "stdafx.h"

namespace esc
{

	class Corner : public sf::Vertex
	{
	public:
		Corner(sf::Vector2f p_v2fPosition, sf::Color p_xColor, int p_iId);

		int getId();

		void setDistanceToSource(sf::Vector2f p_v2fPlayerPosition);
		float getDistanceToSource();

	private:
		int m_iId;

		float m_fDistanceToSource;
	};

}
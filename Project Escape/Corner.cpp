#include "Corner.h"

namespace esc
{

	Corner::Corner(sf::Vector2f p_v2fPosition, sf::Color p_xColor, int p_iId) : Vertex(p_v2fPosition, p_xColor)
	{
		m_iId = p_iId;
	}

	int Corner::getId()
	{
		return m_iId;
	}

	void Corner::setDistanceToSource(sf::Vector2f p_v2fSourcePosition)
	{
		m_fDistanceToSource = sqrtf((p_v2fSourcePosition.x - position.x) * (p_v2fSourcePosition.x - position.x) + (p_v2fSourcePosition.y - position.y) * (p_v2fSourcePosition.y - position.y));
	}

	float Corner::getDistanceToSource()
	{
		return m_fDistanceToSource;
	}

}
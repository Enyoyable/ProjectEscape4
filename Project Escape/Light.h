#pragma once

#include "stdafx.h"
#define _USE_MATH_DEFINES
#include <vector>
#include <map>

namespace esc
{
	class GameObject;

	class Light : public sf::Transformable, public sf::Drawable
	{
	private:

		struct SObjectLines
		{
			esc::GameObject *obj;
			sf::VertexArray lines[4];
		};

	public:
		Light(sf::Vector2f position, sf::Vector2f size, float range, float angle, float maxAlpha, float minAlpha);

		void setObjectsPointer(std::vector<esc::GameObject*> *objects);

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		void setColor(sf::Color color);
		sf::Color *getColor();

		void createVertices();

		void removeUnnessecary();

		void setDirection(sf::Vector2f direction);

		float getLineAngle(sf::VertexArray line);

		void setNewPoints();

		std::vector<sf::VertexArray*> furtherCleaning(std::vector<sf::VertexArray*> *lines, esc::GameObject *obj);

		void createCircle();

	private:
		std::vector<sf::Vertex> m_vertices;

		sf::VertexArray m_circle;

		sf::Color m_color;

		float m_angle;
		float m_maxAlpha;
		float m_minAlpha;
		float m_range;

		std::vector<esc::GameObject*> *m_objects;

		std::vector<sf::VertexArray> m_lines;
		std::vector<sf::VertexArray> m_usefulLines;
		std::vector<sf::VertexArray> m_moreUsefulLines;

		sf::Vector2f m_direction;

		float m_startAngle;

		std::vector<SObjectLines> m_objectLines;

		std::vector<std::vector<sf::VertexArray*>> m_lineIds;
	};

}
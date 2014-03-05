/*#include "Light.h"
#include "GameObject.h"
#include <math.h>

namespace esc
{

	Light::Light(sf::Vector2f position, sf::Vector2f size, float range, float angle, float maxAlpha, float minAlpha)
	{
		setPosition(position);
		m_range = range;
		m_angle = angle;
		m_maxAlpha = maxAlpha;
		m_minAlpha = minAlpha;
		m_color = sf::Color::White;
	}

	void Light::setObjectsPointer(std::vector<esc::GameObject*> *objects)
	{
		m_objects = objects;
	}

	void Light::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{

		for (auto line : m_moreUsefulLines)
		{

			//target.draw(line, states);
		}

		target.draw(m_circle, states);
	}

	void Light::setColor(sf::Color color)
	{
		m_color = color;
	}

	sf::Color *Light::getColor()
	{
		return &m_color;
	}

	void Light::setDirection(sf::Vector2f direction)
	{
		m_direction = direction - getPosition();

		float newAngle = atan2f(m_direction.y, m_direction.x) / 0.017453292519943;

		m_startAngle = (newAngle - m_angle / 2) * -1;

		if (m_startAngle < 0)
		{
			m_startAngle += 360;
		}
		else if (m_startAngle > 360)
		{
			m_startAngle -= 360;
		}
	}

	void Light::createVertices()
	{
		std::vector<sf::Vertex*> objectCorners;
		std::vector<sf::VertexArray> lines;

		m_lines.clear();

		int counter = 0;

		//std::vector<sf::VertexArray*> lines;

		m_lineIds.clear();
		m_moreUsefulLines.clear();

		for (auto object : *m_objects)
		{
			std::vector<sf::VertexArray*> lines;

			for (auto corner : *object->getCorners())
			{
				objectCorners.push_back(corner);

				sf::VertexArray line(sf::Lines, 2);
				line[0].position = getPosition();



				line[1].position = sf::Vector2f(corner->position);

				sf::VertexArray *linePointer = new sf::VertexArray(line);

				lines.push_back(linePointer);

				m_lines.push_back(line);


			}

			m_lineIds.push_back(lines);
			counter++;
		}

		counter = 0;

		for (auto it = m_lines.begin(); it != m_lines.end(); ++it)
		{

			sf::VertexArray line = *it;

			float difference = sqrt((line[1].position.x - getPosition().x) * (line[1].position.x - getPosition().x) + (line[1].position.y - getPosition().y) * (line[1].position.y - getPosition().y));

			if (difference < 200)
			{
				line[0].color = sf::Color::Red;
				line[1].color = sf::Color::Red;
			}
			else
			{
				sf::Color color(0, 0, 0, 0);
				line[0].color = color;
				line[1].color = color;
			}

			m_lines[counter] = line;

			counter++;
		}

		removeUnnessecary();
		setNewPoints();
		createCircle();
	}

	void Light::removeUnnessecary()
	{


		while (m_usefulLines.size() > 0)
		{
			m_usefulLines.erase(m_usefulLines.begin());
		}

		for (auto line : m_lines)
		{
			float lineAngle = getLineAngle(line);

			lineAngle -= m_startAngle;

			if (lineAngle < 0)
			{
				lineAngle += 360;
			}
			else if (lineAngle > 360)
			{
				lineAngle -= 360;
			}

			if (lineAngle > 0 && lineAngle < m_angle)
			{
				m_usefulLines.push_back(line);
			}
		}
	}

	float Light::getLineAngle(sf::VertexArray line)
	{
		sf::Vector2f differnce(line[0].position.x - line[1].position.x, line[0].position.y - line[1].position.y);

		float newAngle = atan2f(differnce.y, differnce.x) / 0.017453292519943 * -1 - 90;

		if (newAngle < 0)
			newAngle += 360;

		if (newAngle > 360)
			newAngle -= 360;

		return newAngle;
	}

	void Light::setNewPoints()
	{
		for (int i = 0; i < m_objects->size(); i++)
		{

			std::vector<sf::VertexArray*> useful = furtherCleaning(&m_lineIds.at(i), m_objects->at(i));

			for (auto lines : useful)
			{
				sf::VertexArray line = *lines;
				float difference = sqrt((line[1].position.x - getPosition().x) * (line[1].position.x - getPosition().x) + (line[1].position.y - getPosition().y) * (line[1].position.y - getPosition().y));

				if (difference < 200)
				{
					float lineAngle = getLineAngle(line);

					lineAngle -= m_startAngle;

					if (lineAngle < 0)
					{
						lineAngle += 360;
					}
					else if (lineAngle > 360)
					{
						lineAngle -= 360;
					}

					if (lineAngle > 0 && lineAngle < m_angle)
					{
						difference *= 255 / 200;
						line[1].color = sf::Color(255, 255, 255, (255 / 200 - difference) / 255.f);
						m_moreUsefulLines.push_back(line);
					}
				}

			}
		}
	}

	std::vector<sf::VertexArray*> Light::furtherCleaning(std::vector<sf::VertexArray*> *lines, GameObject *obj)
	{
		std::vector<float> lineAngles;

		std::vector<sf::VertexArray*> useful;

		if (lines->size() == 0)
			return useful;

		for (auto line : *lines)
		{
			lineAngles.push_back(getLineAngle(*line));
		}

		auto minAngle = std::min_element(lineAngles.begin(), lineAngles.end());

		int minIndex = minAngle - lineAngles.begin();

		auto maxAngle = std::max_element(lineAngles.begin(), lineAngles.end());

		int maxIndex = maxAngle - lineAngles.begin();

		sf::VertexArray minLine = *lines->at(minIndex);
		sf::VertexArray maxLine = *lines->at(maxIndex);

		sf::Vector2f minDiff(fabs(minLine[1].position.x - getPosition().x), fabs(minLine[1].position.y - getPosition().y));

		float minLength = sqrtf(minDiff.x * minDiff.x + minDiff.y * minDiff.y);

		sf::Vector2f maxDiff(fabs(maxLine[1].position.x - getPosition().x), fabs(maxLine[1].position.y - getPosition().y));

		float maxLength = sqrtf(maxDiff.x * maxDiff.x + maxDiff.y * maxDiff.y);

		std::vector<sf::VertexArray*> *goodLines = new std::vector<sf::VertexArray*>();



		for (auto line : *lines)
		{
			if (line == lines->at(minIndex) || line == lines->at(maxIndex))
			{
				continue;
			}

			sf::VertexArray currentLine = *line;

			sf::Vector2f diff(fabs(currentLine[1].position.x - getPosition().x), fabs(currentLine[1].position.y - getPosition().y));

			float diffLength = sqrtf(diff.x * diff.x + diff.y * diff.y);

			if (diffLength < minLength && diffLength < maxLength)
			{
				goodLines->push_back(line);
			}
		}

		/*minLine = *lines->at(minIndex);
		maxLine = *lines->at(maxIndex);

		sf::Vector2f minDiffer(minLine[1].position.x - minLine[0].position.x, minLine[1].position.y - minLine[0].position.y);
		sf::Vector2f maxDiffer(maxLine[1].position.x - maxLine[0].position.x, maxLine[1].position.y - maxLine[0].position.y);

		minDiffer /= (minLength / 200);
		maxDiffer /= (maxLength / 200);

		minLine[0].position.x = getPosition().x;
		minLine[1].position.x = minDiffer.x + getPosition().x;

		minLine[0].position.y = getPosition().y;
		minLine[1].position.y = minDiffer.y + getPosition().y;

		maxLine[0].position.x = getPosition().x;
		maxLine[1].position.x = maxDiffer.x + getPosition().x;

		maxLine[0].position.y = getPosition().y;
		maxLine[1].position.y = maxDiffer.y + getPosition().y;

		sf::VertexArray *minL = new sf::VertexArray(minLine);
		sf::VertexArray *maxL = new sf::VertexArray(maxLine);

		goodLines->push_back(lines->at(minIndex));
		goodLines->push_back(lines->at(maxIndex));

		return *goodLines;
	}

	void Light::createCircle()
	{
		sf::VertexArray circle(sf::TrianglesFan, m_angle + 10);

		circle[0].position = getPosition();

		std::vector<float> lineAngles;

		for (auto line : m_moreUsefulLines)
		{
			lineAngles.push_back(getLineAngle(line));
		}
		float minAngle = 0;
		float maxAngle = 1;

		auto minmaxAngle = std::minmax_element(lineAngles.begin(), lineAngles.end());
		bool noMinMax = false;
		if (lineAngles.size() == 0)
		{
			noMinMax = true;
		}
		else
		{
			minAngle = *minmaxAngle.first;
			maxAngle = *minmaxAngle.second;

			if (maxAngle - minAngle > 180)
			{
				*minmaxAngle.first = maxAngle;
				*minmaxAngle.second = minAngle;
			}
		}


		float currentAngle = 0;
		bool atMin = false, atMax = false;
		for (int i = 0; i < m_angle + 1; i++)
		{
			int angle = currentAngle + m_startAngle;
			if (angle > 360)
			{
				angle -= 360;
			}
			else if (angle < 0)
			{
				angle += 360;
			}
			if (noMinMax)
			{
				float posX = sin(angle * 0.017453292519943) * 200 + getPosition().x;
				float posY = cos(angle *  0.017453292519943) * 200 + getPosition().y;
				circle[i + 1].position.x = posX;
				circle[i + 1].position.y = posY;

				circle[i + 1].color = sf::Color(255, 255, 255, 0);

				currentAngle++;
			}
			else if ((angle < *minmaxAngle.first || angle > *minmaxAngle.second))
			{
				float posX = sin(angle * 0.017453292519943) * 200 + getPosition().x;
				float posY = cos(angle *  0.017453292519943) * 200 + getPosition().y;
				circle[i + 1].position.x = posX;
				circle[i + 1].position.y = posY;

				circle[i + 1].color = sf::Color(255, 255, 255, 0);
				currentAngle++;
			}
			else if ((angle > *minmaxAngle.first || angle < *minmaxAngle.second) && !atMin)
			{
				/*atMin = true;
				float posX = sin(*minmaxAngle.first * 0.017453292519943) * 200 + getPosition().x;
				float posY = cos(*minmaxAngle.first * 0.017453292519943) * 200 + getPosition().y;
				circle[i + 1].position.x = posX;
				circle[i++ + 1].position.y = posY;

				if (lineAngles.size() == 3)
				{
				for (auto it = lineAngles.begin(); it != lineAngles.end(); ++it)
				{
				if (*it > *minmaxAngle.first && *it < *minmaxAngle.second)
				{
				int index = it - lineAngles.begin();

				circle[i++ + 1].position = m_moreUsefulLines.at(index)[1].position;
				}
				}
				}

				posX = sin(*minmaxAngle.second * 0.017453292519943) * 200 + getPosition().x;
				posY = cos(*minmaxAngle.second * 0.017453292519943) * 200 + getPosition().y;
				circle[i + 1].position.x = posX;
				circle[i + 1].position.y = posY;

				//currentAngle = *minmaxAngle.second;
				currentAngle++;
			}
			else
			{
				float posX = sin(angle * 0.017453292519943) * 200 + getPosition().x;
				float posY = cos(angle *  0.017453292519943) * 200 + getPosition().y;
				circle[i + 1].position.x = posX;
				circle[i + 1].position.y = posY;

				circle[i + 1].color = sf::Color(255, 255, 255, 0);
				currentAngle++;
			}

		}

		for (int i = 0; i < circle.getVertexCount(); i++)
		{
			if ((int)circle[i].position.x == 0 && (int)circle[i].position.y == 0)
			{
				circle[i].position = getPosition();
			}
		}

		m_circle = circle;
	}

}*/
#include "LightSource.h"
#include "GameObject.h"
#include "Corner.h"

namespace esc
{

	LightSource::LightSource(sf::Vector2f p_v2fPosition, float p_fRadious, std::vector<GameObject*> *p_vObjects, float p_fAngleWidth)
	{
		setPosition(p_v2fPosition);
		m_fRadious = p_fRadious;
		m_vObjects = p_vObjects;

		m_fLightWidthAngle = p_fAngleWidth;
	}

	LightSource::LightSource(sf::Vector2f p_v2fPosition, sf::Vector2f p_v2fSize, float p_fRadious, std::vector<GameObject*> *p_vObjects, float p_fAngleWidth)
	{
		setPosition(p_v2fPosition);
		m_fRadious = p_fRadious;
		m_vObjects = p_vObjects;
		m_v2fSize = p_v2fSize;

		m_fLightWidthAngle = p_fAngleWidth;
		m_fMiddleAngle = 0;
		m_fStartAngle = m_fMiddleAngle - m_fLightWidthAngle / 2;
	}
	
	void LightSource::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		sf::VertexArray triangleFan(sf::TrianglesFan, m_vTriangleFan.size());

		for (int i = 0; i < m_vTriangleFan.size(); i++)
		{
			triangleFan[i] = *m_vTriangleFan[i];
		}

		target.draw(triangleFan, states);
	}

	void LightSource::setRadious(float p_fRadious)
	{
		m_fRadious = p_fRadious;
	}

	float LightSource::getRadious()
	{
		return m_fRadious;
	}

	void LightSource::setDirection(sf::Vector2f p_v2fDirection)
	{
		sf::Vector2f direction(getPosition().x - p_v2fDirection.x, getPosition().y - p_v2fDirection.y);

		m_fMiddleAngle = (float)(atan2f(direction.y, direction.x) / 0.017453292519943) + 180;
		m_fStartAngle = m_fMiddleAngle - m_fLightWidthAngle / 2;

		if (m_fStartAngle < 0)
		{
			m_fStartAngle += 360;
		}
		else if (m_fStartAngle > 360)
		{
			m_fStartAngle -= 360;
		}
	}

	float LightSource::getDirectionAngle()
	{
		return m_fMiddleAngle;
	}

	void LightSource::update(float p_fDeltaTime)
	{
		createLight();
	}

	void LightSource::createLight()
	{
		m_vTriangleFan.clear();
		

		std::vector<Corner*> relevantCorners;

		std::map<Corner*, float> cornerAngles;

		getRelevantCorners(&relevantCorners, &cornerAngles);

		createFan(&relevantCorners, &cornerAngles);

		m_vCorners = relevantCorners;
	}

	void LightSource::getRelevantCorners(std::vector<Corner*> *p_vCorners, std::map<Corner*, float> *p_mCornerAngles)
	{
		m_vCorners.clear();

		for (auto object : *m_vObjects)
		{
			

			for (auto corner : object->getCorners())
			{
				
				corner->color = sf::Color(255, 255, 255, 1 - corner->getDistanceToSource() / m_fRadious);
				corner->setDistanceToSource(getPosition());
				if (corner->getDistanceToSource() > m_fRadious)
					continue;
				m_vCorners.push_back(corner);

				float fCornerAngle = getCornerAngle(corner);

				p_mCornerAngles->insert(std::pair<Corner*, float>(corner, fCornerAngle));
			}
		}

		for (auto corner : m_vCorners)
		{
			if (m_fRadious >= corner->getDistanceToSource())
			{
				float fEndAngle = m_fStartAngle + m_fLightWidthAngle;

				if (fEndAngle > 360)
					fEndAngle -= 360;

				float cornerAngle = p_mCornerAngles->find(corner)->second;

				if (m_fStartAngle > fEndAngle)
				{
					if (cornerAngle > m_fStartAngle || cornerAngle < fEndAngle)
					{
						p_vCorners->push_back(corner);
					}
				}
				else
				{
					if (cornerAngle > m_fStartAngle && cornerAngle < fEndAngle)
					{
						p_vCorners->push_back(corner);
					}
				}
				
			}
		}

		std::vector<Corner*> vCleanedCorners;
		std::vector<float> vDistanceToObject;

		std::vector<GameObject*> vSortedObjects;

		std::vector<GameObject*> vUnsortedObjects = *m_vObjects;

		std::vector<SDeadSector> vDeadSectors;

		float fShortest = m_fRadious;

		for (auto object : *m_vObjects)
		{
			vDistanceToObject.push_back(sqrtf((object->getPosition().x + object->getSize().x * 0.5f - getPosition().x) * (object->getPosition().x + object->getSize().x * 0.5f - getPosition().x) + (object->getPosition().y + object->getSize().y * 0.5f - getPosition().y) * (object->getPosition().y + object->getSize().y * 0.5f - getPosition().y)));
		}

		int size = vUnsortedObjects.size();

		for (int i = 0; i < size; i++)
		{
			auto iter = std::min_element(vDistanceToObject.begin(), vDistanceToObject.end());
			int index = iter - vDistanceToObject.begin();

			vSortedObjects.push_back(vUnsortedObjects[index]);

			auto removeIter = vUnsortedObjects.begin() + index;

			vDistanceToObject.erase(iter);
			vUnsortedObjects.erase(removeIter);
		}

		for (auto object : vSortedObjects)
		{
			float fDistToObject = sqrtf((object->getPosition().x + object->getSize().x * 0.5f - getPosition().x) * (object->getPosition().x + object->getSize().x * 0.5f - getPosition().x) + (object->getPosition().y + object->getSize().y * 0.5f - getPosition().y) * (object->getPosition().y + object->getSize().y * 0.5f - getPosition().y));
			std::vector<Corner*> vObjCorners = object->getCorners();

			SDeadSector DeadAddition;

			std::vector<float> vCornerAngles;

			for (auto corner : vObjCorners)
			{
				if (p_mCornerAngles->find(corner) != p_mCornerAngles->end())
				{
					vCornerAngles.push_back(p_mCornerAngles->find(corner)->second);
				}
				else
				{
					continue;
				}
					
				
			}

			if (vCornerAngles.size() == 0)
			{
				continue;
			}

			auto itMinAngle = std::min_element(vCornerAngles.begin(), vCornerAngles.end());
			float fMinAngle = *itMinAngle;

			auto itMaxAngle = std::max_element(vCornerAngles.begin(), vCornerAngles.end());
			float fMaxAngle = *itMaxAngle;

			if (fMaxAngle - fMinAngle > 180)
			{
				std::vector<float> vCorrMinMax;
				for (auto angle : vCornerAngles)
				{
					if (!fabs(angle - fMinAngle) < 0.00001 && !fabs(angle - fMaxAngle) < 0.00001)
					{
						if (angle < fMaxAngle && angle > fMinAngle)
						{
							vCorrMinMax.push_back(angle);
						}
					}
				}

				if (vCorrMinMax.size() == 0)
					continue;

				fMaxAngle = *std::min_element(vCorrMinMax.begin(), vCorrMinMax.end());
				fMinAngle = *std::max_element(vCorrMinMax.begin(), vCorrMinMax.end());
			}
			float fDistToMinCorner = 0;
			float fDistToMaxCorner = 0;


			for (auto corner : vObjCorners)
			{
				if (p_mCornerAngles->find(corner) == p_mCornerAngles->end())
					continue;
				float cornerAngle = p_mCornerAngles->find(corner)->second;
				float fDistToCorner = sqrtf((corner->position.x - getPosition().x) * (corner->position.x - getPosition().x) + (corner->position.y - getPosition().y) * (corner->position.y - getPosition().y));
				if (fabs(cornerAngle - fMinAngle) < 0.00001 && fabs(cornerAngle - fMinAngle) < 0.00001)
				{
					fDistToMinCorner = fDistToCorner;
				}
				else if (fabs(cornerAngle - fMaxAngle) < 0.00001 && fabs(cornerAngle - fMaxAngle) < 0.00001)
				{
					fDistToMaxCorner = fDistToCorner;
				}
			}

			DeadAddition.m_fFinishAngle = fMaxAngle;
			DeadAddition.m_fStartingAngle = fMinAngle;

			std::vector<Corner*> vCornersOutsideSector;

			if (vDeadSectors.size() > 0)
			{
				for (auto corner : vObjCorners)
				{
					float fDistToCorner = sqrtf((corner->position.x - getPosition().x) * (corner->position.x - getPosition().x) + (corner->position.y - getPosition().y) * (corner->position.y - getPosition().y));
					if (m_fRadious < fDistToCorner)
					{
						continue;
					}
					bool bInSector = false;
					float cornerAngle = p_mCornerAngles->find(corner)->second;

					if (fabs(cornerAngle - fMinAngle) < 0.00001 && fabs(cornerAngle - fMinAngle) < 0.00001)
					{
						fDistToMinCorner = fDistToCorner;
						for (auto sector : vDeadSectors)
						{
							if (sector.m_fStartingAngle > sector.m_fFinishAngle)
							{
								if (cornerAngle > sector.m_fStartingAngle || cornerAngle < sector.m_fFinishAngle)
								{
									bInSector = true;
								}
							}
							else
							{
								if (cornerAngle > sector.m_fStartingAngle && cornerAngle < sector.m_fFinishAngle)
								{
									bInSector = true;
								}
							}
						}

						if (!bInSector)
						{
							vCleanedCorners.push_back(corner);
							vCornersOutsideSector.push_back(corner);
						}
					}
					else if (fabs(cornerAngle - fMaxAngle) < 0.00001 && fabs(cornerAngle - fMaxAngle) < 0.00001)
					{
						fDistToMaxCorner = fDistToCorner;
						for (auto sector : vDeadSectors)
						{
							if (sector.m_fStartingAngle > sector.m_fFinishAngle)
							{
								if (cornerAngle > sector.m_fStartingAngle || cornerAngle < sector.m_fFinishAngle)
								{
									bInSector = true;
								}
							}
							else
							{
								if (cornerAngle > sector.m_fStartingAngle && cornerAngle < sector.m_fFinishAngle)
								{
									bInSector = true;
								}
							}
						}

						if (!bInSector)
						{
							vCleanedCorners.push_back(corner);
							vCornersOutsideSector.push_back(corner);
						}
					}
					else if (fabs(fDistToCorner) < fabs(fDistToMaxCorner) && fabs(fDistToCorner) < fabs(fDistToMinCorner))
					{
						for (auto sector : vDeadSectors)
						{
							if (sector.m_fStartingAngle > sector.m_fFinishAngle)
							{
								if (cornerAngle > sector.m_fStartingAngle || cornerAngle < sector.m_fFinishAngle)
								{
									bInSector = true;
								}
							}
							else
							{
								if (cornerAngle > sector.m_fStartingAngle && cornerAngle < sector.m_fFinishAngle)
								{
									bInSector = true;
								}
							}
						}

						if (!bInSector)
						{
							vCleanedCorners.push_back(corner);
							vCornersOutsideSector.push_back(corner);
						}
					}

				}
			}
			else
			{
				for (auto corner : vObjCorners)
				{
					float fDistToCorner = sqrtf((corner->position.x - getPosition().x) * (corner->position.x - getPosition().x) + (corner->position.y - getPosition().y) * (corner->position.y - getPosition().y));
					if (m_fRadious < fDistToCorner)
					{
						continue;
					}

					if (p_mCornerAngles->find(corner) != p_mCornerAngles->end())
					{
						float cornerAngle = p_mCornerAngles->find(corner)->second;

						if (fabs(cornerAngle - fMinAngle) < 0.00001 && fabs(cornerAngle - fMinAngle) < 0.00001)
						{
							vCleanedCorners.push_back(corner);
							vCornersOutsideSector.push_back(corner);
						}
						else if (fabs(cornerAngle - fMaxAngle) < 0.00001 && fabs(cornerAngle - fMaxAngle) < 0.00001)
						{
							vCleanedCorners.push_back(corner);
							vCornersOutsideSector.push_back(corner);
						}
						else if (fabs(fDistToCorner) < fabs(fDistToMaxCorner) && fabs(fDistToCorner) < fabs(fDistToMinCorner))
						{
							vCleanedCorners.push_back(corner);
							vCornersOutsideSector.push_back(corner);
						}
					}
				}
			}

			if (vCornersOutsideSector.size() > 0)
			{
				printf("Obj id: %i   Amount of corners outside sector: %i\n", object->getId(), vCornersOutsideSector.size());

				if (vCornersOutsideSector.size() == 1)
				{

				}
			}

			vDeadSectors.push_back(DeadAddition);

		}

		*p_vCorners = vCleanedCorners;
	
	}

	void LightSource::createFan(std::vector<Corner*> *p_vCorners, std::map<Corner*, float> *p_mCornerAngles)
	{
		if (p_vCorners->size() == 0)
		{
			return;
		}

		sf::Vertex *playerVertex = new sf::Vertex(getPosition(), sf::Color::White);

		m_vTriangleFan.clear();

		m_vTriangleFan.push_back(playerVertex);

		std::map<float, Corner*> mCornerAngles;
		std::vector<float> vCornerAngles;

		for (auto iter = p_mCornerAngles->rbegin(); iter != p_mCornerAngles->rend(); ++iter)
		{
			if (mCornerAngles.find(iter->second) != mCornerAngles.end())
			{
				continue;
			}

			mCornerAngles.insert(std::pair<float, Corner*>(iter->second, iter->first)); 
			vCornerAngles.push_back(iter->second);
		}
		int maxCount = 0;

		bool bHasPassedZero = false;

		float fLastAngle = m_fStartAngle;

		for (float fCurrentAngle = m_fStartAngle; true; fCurrentAngle += 1)
		{
			if (fCurrentAngle > 360)
			{
				fCurrentAngle -= 360;
			}

			if (fLastAngle > fCurrentAngle)
			{
				bHasPassedZero = true;
			}

			if (bHasPassedZero && fCurrentAngle < 1)
			{
				//return;
			}

			fLastAngle = fCurrentAngle;
			maxCount++;

			if (maxCount > 360)
			{
				return;
			}

			float endAngle = m_fStartAngle + m_fLightWidthAngle;

			if (endAngle > 359.99)
			{
				endAngle -= 360.01;
			}

			if (m_fStartAngle > endAngle)
			{
				if (bHasPassedZero && fCurrentAngle > endAngle)
				{
					if (fabs(m_fLightWidthAngle - 360) < 0.001)
					{
						sf::Vertex *vertex = new sf::Vertex(*m_vTriangleFan[1]);
						m_vTriangleFan.push_back(vertex);
					}
					return;
				}
			}
			else if (fCurrentAngle > endAngle)
			{
				return;
			}

			bool bCornerInRange = false;
			for (auto cornerAngle : vCornerAngles)
			{
				if (fCurrentAngle < cornerAngle && fCurrentAngle + 1 > cornerAngle)
				{
					bCornerInRange = true;
					fCurrentAngle = cornerAngle;
					Corner *xCorner = mCornerAngles.find(fCurrentAngle)->second;

					xCorner->color = sf::Color(255, 255, 255, (1.f - xCorner->getDistanceToSource() / m_fRadious) * 255);

					std::vector<float> vCurrCornerAngles;

					if (xCorner->position != getPosition())
						m_vTriangleFan.push_back(xCorner);

					std::vector<Corner*> additionalCorners;
					

					for (auto corner : *p_vCorners)
					{
						float fCornerAngle = p_mCornerAngles->find(corner)->second;
						if (corner->getId() == xCorner->getId() && corner != xCorner && corner->getDistanceToSource() < m_fRadious && corner->position != getPosition())// && fCurrentAngle < fCornerAngle)
						{
							//float fDiff = fabs(fCurrentAngle - fCornerAngle);
							if (fCurrentAngle > fCornerAngle)
							{
								//printf("Current Angle: %f\nCorner Angle: %f\n", fCurrentAngle, fCornerAngle);
							}

							/*if (fDiff > 180)
							{
								if (fCurrentAngle > fCornerAngle)
								{
									corner->color = sf::Color(255, 255, 255, (1.f - corner->getDistanceToSource() / m_fRadious) * 255);
									additionalCorners.push_back(corner);
									vCurrCornerAngles.push_back(getCornerAngle(corner));
									//fCurrentAngle = fCornerAngle;
								}
							}
							else if (fCurrentAngle < fCornerAngle)*/
							{
								corner->color = sf::Color(255, 255, 255, (1.f - corner->getDistanceToSource() / m_fRadious) * 255);
								additionalCorners.push_back(corner);
								vCurrCornerAngles.push_back(getCornerAngle(corner));
							}
							
						}
					}

					if (vCurrCornerAngles.size() != 0 && additionalCorners.size() != 0)
					{
						auto additionalAnglesIter = std::minmax_element(vCurrCornerAngles.begin(), vCurrCornerAngles.end());

						if (*additionalAnglesIter.first == *additionalAnglesIter.second && additionalCorners.size() > 0)
						{
							m_vTriangleFan.push_back(additionalCorners[0]);

							fCurrentAngle = *additionalAnglesIter.first;
						}
						else if (additionalCorners.size() == 3)
						{
							int xFirstIndex = additionalAnglesIter.first - vCurrCornerAngles.begin();
							int xSecondIndex = additionalAnglesIter.second - vCurrCornerAngles.begin();

							int iMiddleIndex;

							for (int i = 0; i < 3; i++)
							{
								if (i != xFirstIndex && i != xSecondIndex)
								{
									iMiddleIndex = i;
								}
							}

							m_vTriangleFan.push_back(additionalCorners[xFirstIndex]);
							m_vTriangleFan.push_back(additionalCorners[iMiddleIndex]);
							m_vTriangleFan.push_back(additionalCorners[xSecondIndex]);

							fCurrentAngle = *additionalAnglesIter.second;
						}
						else if (*additionalAnglesIter.first < *additionalAnglesIter.second)
						{
							int xFirstIndex = additionalAnglesIter.first - vCurrCornerAngles.begin();
							int xSecondIndex = additionalAnglesIter.second - vCurrCornerAngles.begin();

							float firstAngle = *additionalAnglesIter.first;
							float secondAngle = *additionalAnglesIter.second;

							if (firstAngle < secondAngle)
							{
								if (secondAngle - firstAngle > 180)
								{
									if (fCurrentAngle < firstAngle)
									{
										//m_vTriangleFan.push_back(additionalCorners[xSecondIndex]);
										m_vTriangleFan.push_back(additionalCorners[xFirstIndex]);
										
										fCurrentAngle = firstAngle;
									}
									else if (fCurrentAngle > secondAngle)
									{
										//m_vTriangleFan.push_back(additionalCorners[xFirstIndex]);
										m_vTriangleFan.push_back(additionalCorners[xSecondIndex]);
										
										fCurrentAngle = firstAngle - 0.1;
									}
								}
								else
								{
									m_vTriangleFan.push_back(additionalCorners[xFirstIndex]);
									m_vTriangleFan.push_back(additionalCorners[xSecondIndex]);

									fCurrentAngle = *additionalAnglesIter.second;
								}
							}
							else if (secondAngle < firstAngle)
							{
								if (secondAngle - firstAngle > 180)
								{
									if (fCurrentAngle < firstAngle)
									{
										m_vTriangleFan.push_back(additionalCorners[xSecondIndex]);
										fCurrentAngle = firstAngle;
									}
									else if (fCurrentAngle > secondAngle)
									{
										m_vTriangleFan.push_back(additionalCorners[xFirstIndex]);
										fCurrentAngle = secondAngle;
									}
								}
								else
								{
									m_vTriangleFan.push_back(additionalCorners[xSecondIndex]);
									m_vTriangleFan.push_back(additionalCorners[xFirstIndex]);

									fCurrentAngle = *additionalAnglesIter.second;
								}
							}

							

							
						}
						else if (*additionalAnglesIter.first > *additionalAnglesIter.second)
						{
							int xFirstIndex = additionalAnglesIter.second - vCurrCornerAngles.begin();
							int xSecondIndex = additionalAnglesIter.first - vCurrCornerAngles.begin();

							m_vTriangleFan.push_back(additionalCorners[xFirstIndex]);
							m_vTriangleFan.push_back(additionalCorners[xSecondIndex]);

							fCurrentAngle = *additionalAnglesIter.second;
						}
					}
					
				}
			}

			if (bCornerInRange == false)
			{
				sf::Vertex *vertex = new sf::Vertex(sf::Vector2f(cosf(fCurrentAngle * 0.017453292519943) * m_fRadious, sinf(fCurrentAngle * 0.017453292519943) * m_fRadious) + getPosition(), sf::Color(255, 255, 255, 0));

				if (vertex->position != getPosition())
				{
					m_vTriangleFan.push_back(vertex);

				}
				else
				{
					delete vertex;
					vertex = nullptr;
				}

				
			}

			if (fCurrentAngle > m_fStartAngle + m_fLightWidthAngle)
				break;

		}
	}

	float LightSource::getCornerAngle(Corner* p_xCorner)
	{
		sf::Vector2f difference(getPosition().x - p_xCorner->position.x, getPosition().y - p_xCorner->position.y);

		return (float)(atan2f(difference.y, difference.x) / 0.017453292519943) + 180;
	}

}
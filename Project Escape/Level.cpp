#include "Level.h"
#include "SpriteManager.h"
#include "GameObjectManager.h"
#include "Guard.h"
#include "AnimatedSprite.h"
#include "GameObject.h"
#include "PlayerObject.h"
#include <fstream>
#include "PathNode.h"
#include "PathFind.h"
#include "Item.h"
#include "AIManager.h"
#include "Door.h"

namespace esc
{

	Level::Level(std::string p_sDirectoryPath, SpriteManager *p_xSpriteManager, GameObjectManager *p_xGameObjectManager)
	{
		m_sDirectoryPath = p_sDirectoryPath;
		m_xSpriteManager = p_xSpriteManager;
		m_xGameObjectManager = p_xGameObjectManager;

		m_iStateNum = 0;

		for (int x = 0; x < 200; x++)
		{
			for (int y = 0; y < 200; y++)
			{
				m_aAdjacantObjects[x][y] = nullptr;
				m_aWalls[x][y] = nullptr;
			}
		}


	}

	void Level::update(float p_fDeltaTime)
	{
		m_xGameObjectManager->updateObjects(&m_vLevelObjects, p_fDeltaTime);
	}

	void Level::loadColorCodes(std::string p_sFilePath)
	{
		std::fstream stream(m_sDirectoryPath + p_sFilePath);

		while (!stream.eof())
		{
			int r, g, b, a;

			stream >> r >> g >> b >> a;

			std::string sObjectIdentifier;

			stream >> sObjectIdentifier;

			EObjectType eObjectIdentifier;

			if (sObjectIdentifier.compare("PATROLLINGGUARD") == 0)
				eObjectIdentifier = EObjectType::PATROLLINGGUARD;
			else if (sObjectIdentifier.compare("STATIONARYGUARD") == 0)
				eObjectIdentifier = EObjectType::STATIONARYGUARD;
			else if (sObjectIdentifier.compare("WALL") == 0)
				eObjectIdentifier = EObjectType::WALL;
			else if (sObjectIdentifier.compare("TOILET") == 0)
				eObjectIdentifier = EObjectType::TOILET;
			else if (sObjectIdentifier.compare("URINAL") == 0)
				eObjectIdentifier = EObjectType::URINAL;
			else if (sObjectIdentifier.compare("SINK") == 0)
				eObjectIdentifier = EObjectType::SINK;
			else if (sObjectIdentifier.compare("HIDINGUP") == 0)
				eObjectIdentifier = EObjectType::LOCKERU;
			else if (sObjectIdentifier.compare("HIDINGR") == 0)
				eObjectIdentifier = EObjectType::LOCKERR;
			else if (sObjectIdentifier.compare("HIDINGL") == 0)
				eObjectIdentifier = EObjectType::LOCKERL;
			else if (sObjectIdentifier.compare("HIDINGD") == 0)
				eObjectIdentifier = EObjectType::LOCKERD;
			else if (sObjectIdentifier.compare("EXIT") == 0)
				eObjectIdentifier = EObjectType::EXIT;
			else if (sObjectIdentifier.compare("DESKS") == 0)
				eObjectIdentifier = EObjectType::DESKS;
			else if (sObjectIdentifier.compare("COUCH") == 0)
				eObjectIdentifier = EObjectType::COUCH;
			else if (sObjectIdentifier.compare("BATON") == 0)
				eObjectIdentifier = EObjectType::BATON;
			else if (sObjectIdentifier.compare("GUN") == 0)
				eObjectIdentifier = EObjectType::GUN;
			else if (sObjectIdentifier.compare("RADIO") == 0)
				eObjectIdentifier = EObjectType::RADIO;
			else if (sObjectIdentifier.compare("KEYCARD") == 0)
				eObjectIdentifier = EObjectType::KEYCARD;
			else if (sObjectIdentifier.compare("PDA") == 0)
				eObjectIdentifier = EObjectType::PDA;
			else if (sObjectIdentifier.compare("DOOR") == 0)
				eObjectIdentifier = EObjectType::DOOR;
			else if (sObjectIdentifier.compare("LOCKEDH") == 0)
				eObjectIdentifier = EObjectType::LOCKEDH;
			else if (sObjectIdentifier.compare("LOCKEDV") == 0)
				eObjectIdentifier = EObjectType::LOCKEDV;
			else if (sObjectIdentifier.compare("OPENH") == 0)
				eObjectIdentifier = EObjectType::OPENH;
			else if (sObjectIdentifier.compare("OPENV") == 0)
				eObjectIdentifier = EObjectType::OPENV;
			else if (sObjectIdentifier.compare("CLOSEDH") == 0)
				eObjectIdentifier = EObjectType::CLOSEDH;
			else if (sObjectIdentifier.compare("CLOSEDV") == 0)
				eObjectIdentifier = EObjectType::CLOSEDV;
			else if (sObjectIdentifier.compare("WRITER") == 0)
				eObjectIdentifier = EObjectType::WRITER;
			else if (sObjectIdentifier.compare("PLANTS") == 0)
				eObjectIdentifier = EObjectType::PLANTS;
			else if (sObjectIdentifier.compare("RECMID") == 0)
				eObjectIdentifier = EObjectType::RECMID;
			else if (sObjectIdentifier.compare("RECTOP") == 0)
				eObjectIdentifier = EObjectType::RECTOP;
			else if (sObjectIdentifier.compare("RECLOW") == 0)
				eObjectIdentifier = EObjectType::RECLOW;
			else if (sObjectIdentifier.compare("FIKABORD") == 0)
				eObjectIdentifier = EObjectType::FIKABORD;
			else if (sObjectIdentifier.compare("STOLARUPP") == 0)
				eObjectIdentifier = EObjectType::STOLARUPP;
			else if (sObjectIdentifier.compare("STOLARNER") == 0)
				eObjectIdentifier = EObjectType::STOLARNER;
			else if (sObjectIdentifier.compare("STOLARV") == 0)
				eObjectIdentifier = EObjectType::STOLARV;
			else if (sObjectIdentifier.compare("STOLARH") == 0)
				eObjectIdentifier = EObjectType::STOLARH;
			else if (sObjectIdentifier.compare("OBJECTIVE") == 0)
				eObjectIdentifier = EObjectType::OBJECTIVE;


			sf::Color *color = new sf::Color(r, g, b, a);

			m_vColorCodes.push_back(color);

			m_mColorCodes.insert(std::pair<sf::Color*, EObjectType>(color, eObjectIdentifier));
		}

	}

	void Level::loadFloorColorCodes(std::string p_sFilePath)
	{
		std::fstream stream(m_sDirectoryPath + p_sFilePath);

		while (!stream.eof())
		{
			int r, g, b, a;

			stream >> r >> g >> b >> a;

			std::string sObjectIdentifier;

			stream >> sObjectIdentifier;

			EFloorType eFloorType;

			if (sObjectIdentifier.compare("FLOOR") == 0)
				eFloorType = EFloorType::NORMAL;
			else if (sObjectIdentifier.compare("NORMALFLOOR") == 0)
				eFloorType = EFloorType::NORMALFLOOR;
			else if (sObjectIdentifier.compare("FLOOR2") == 0)
				eFloorType = EFloorType::FLOOR2;
			else if (sObjectIdentifier.compare("FLOOR3") == 0)
				eFloorType = EFloorType::FLOOR3;
			else if (sObjectIdentifier.compare("FLOOR4") == 0)
				eFloorType = EFloorType::FLOOR4;

			sf::Color *color = new sf::Color(r, g, b, a);

			m_vFloorColorCodes.push_back(color);

			m_mFloorColorCodes.insert(std::pair<sf::Color*, EFloorType>(color, eFloorType));
		}

	}

	void Level::create(std::string p_sFilePath, std::string p_sPatrolPath)
	{
		sf::Image xImage;

		xImage.loadFromFile(m_sDirectoryPath + p_sFilePath);

		int iCurrentGuardCount = 0;

		int iCurrentRotationCount = 0;

		std::vector<Guard*> vPatrollingGuards;

		for (int y = 0; y < xImage.getSize().y; y++)
		{

			std::vector<PathNode*> vPathNodes;

			for (int x = 0; x < xImage.getSize().x; x++)
			{
				PathNode *xNode = new PathNode(sf::Vector2i(x, y), false);
				sf::Color currentColor = xImage.getPixel(x, y);

				sf::Color *colorPointer = nullptr;

				for (auto color : m_vColorCodes)
				{
					if (currentColor == *color)
					{
						colorPointer = color;
						break;
					}

				}

				auto iterCurrentObjectType = m_mColorCodes.find(colorPointer);

				if (iterCurrentObjectType == m_mColorCodes.end())
				{
					vPathNodes.push_back(xNode);
					continue;
				}

				switch (iterCurrentObjectType->second)
				{
				case esc::WALL:
					{
						GameObject *obj = m_xGameObjectManager->createObject(sf::Vector2f(x * 64, y * 64), sf::Vector2f(64, 64), false, EObjectType::WALL, nullptr);
						m_vLevelObjects.push_back(obj);
						m_aAdjacantObjects[x][y] = obj;
						m_aWalls[x][y] = obj;
						xNode->setIllegal(true);
						break;

					}
				case esc::COUCH:
					{
						GameObject *obj = m_xGameObjectManager->createObject(sf::Vector2f(x * 64, y * 64), sf::Vector2f(64, 64), false, EObjectType::COUCH, nullptr);
						m_vLevelObjects.push_back(obj);
						m_aAdjacantObjects[x][y] = obj;
						xNode->setIllegal(true);
						break;
					}
				case esc::LOCKERD:
					{
						GameObject *obj = m_xGameObjectManager->createObject(sf::Vector2f(x * 64, y * 64), sf::Vector2f(64, 64), true, EObjectType::LOCKERD, m_xSpriteManager->loadSprite("LockerD.png", 0, 0, 64, 64));
						m_vLevelObjects.push_back(obj);
						xNode->setIllegal(true);
						break;
					}

				case esc::LOCKERU:
					{
						GameObject *obj = m_xGameObjectManager->createObject(sf::Vector2f(x * 64, y * 64), sf::Vector2f(64, 64), true, EObjectType::LOCKERU, m_xSpriteManager->loadSprite("LockerU.png", 0, 0, 64, 64));
						m_vLevelObjects.push_back(obj);
						xNode->setIllegal(true);
						break;
					}

				case esc::LOCKERL:
					{
						GameObject *obj = m_xGameObjectManager->createObject(sf::Vector2f(x * 64, y * 64), sf::Vector2f(64, 64), true, EObjectType::LOCKERL, m_xSpriteManager->loadSprite("LockerL.png", 0, 0, 64, 64));
						m_vLevelObjects.push_back(obj);
						xNode->setIllegal(true);
						break;
					}

				case esc::LOCKERR:
					{
						GameObject *obj = m_xGameObjectManager->createObject(sf::Vector2f(x * 64, y * 64), sf::Vector2f(64, 64), true, EObjectType::LOCKERR, m_xSpriteManager->loadSprite("LockerR.png", 0, 0, 64, 64));
						m_vLevelObjects.push_back(obj);
						xNode->setIllegal(true);
						break;
					}

				case esc::PLAYER:
					{
						break;
					}
				case esc::TOILET:
				{
									GameObject *obj = m_xGameObjectManager->createObject(sf::Vector2f(x * 64, y * 64), sf::Vector2f(64, 64), false, EObjectType::WALL, m_xSpriteManager->loadSprite("Toalett_NoBackF.png", 0, 0, 64, 64));
									m_vLevelObjects.push_back(obj);
									xNode->setIllegal(true);
									break;
				}
				case esc::PDA:
				{
									GameObject *obj = m_xGameObjectManager->createObject(sf::Vector2f(x * 64, y * 64), sf::Vector2f(64, 64), false, EObjectType::WALL, m_xSpriteManager->loadSprite("Toalett_NoBackF.png", 0, 0, 64, 64));
									m_vLevelObjects.push_back(obj);
									obj->setAnimation(m_xSpriteManager->loadAnimatedSprite("PDAanim.txt"));
									break;
				}
				case esc::URINAL:
					{
						GameObject *obj = m_xGameObjectManager->createObject(sf::Vector2f(x * 64, y * 64), sf::Vector2f(64, 64), false, EObjectType::WALL, m_xSpriteManager->loadSprite("Pissoar_NoBackL.png", 0, 0, 64, 64));
						m_vLevelObjects.push_back(obj);
						xNode->setIllegal(true);
						break;
					}
				case esc::URINAL2:
					{
						GameObject *obj = m_xGameObjectManager->createObject(sf::Vector2f(x * 64, y * 64), sf::Vector2f(64, 64), false, EObjectType::WALL, m_xSpriteManager->loadSprite("Pissoar_NoBackR.png", 0, 0, 64, 64));
						m_vLevelObjects.push_back(obj);
						xNode->setIllegal(true);
						break;
					}
				case esc::SINK:
					{
						GameObject *obj = m_xGameObjectManager->createObject(sf::Vector2f(x * 64, y * 64), sf::Vector2f(64, 64), false, EObjectType::WALL, m_xSpriteManager->loadSprite("Sink.png", 0, 0, 64, 64));
						m_vLevelObjects.push_back(obj);
						xNode->setIllegal(true);
						break;
					}
				case esc::DESKS:
					{
						GameObject *obj = m_xGameObjectManager->createObject(sf::Vector2f(x * 64, y * 64), sf::Vector2f(64, 64), false, EObjectType::DESKS, m_xSpriteManager->loadSprite("desk1down.png", 0, 0, 64, 64));
						m_vLevelObjects.push_back(obj);
						m_aAdjacantObjects[x][y] = obj;
						xNode->setIllegal(true);
						break;
					}
				case esc::PLANTS:
					{
						GameObject *obj = m_xGameObjectManager->createObject(sf::Vector2f(x * 64, y * 64), sf::Vector2f(64, 64), false, EObjectType::PLANTS, m_xSpriteManager->loadSprite("flower_pot.png", 0, 0, 64, 64));
						m_vLevelObjects.push_back(obj);
						break;
					}
				case esc::WRITER:
					{
						GameObject *obj = m_xGameObjectManager->createObject(sf::Vector2f(x * 64, y * 64), sf::Vector2f(64, 64), false, EObjectType::WRITER, m_xSpriteManager->loadSprite("printer.png", 0, 0, 64, 64));
						m_vLevelObjects.push_back(obj);
						break;
					}
				case esc::EXIT:
					{
						GameObject *obj = m_xGameObjectManager->createObject(sf::Vector2f(x * 64, y * 64), sf::Vector2f(64, 64), false, EObjectType::EXIT, m_xSpriteManager->loadSprite("Exit.png", 0, 0, 64, 64));
						m_vLevelObjects.push_back(obj);
						break;
					}
				case esc::STATIONARYGUARD:
					{
						sf::Sprite *sprite = m_xSpriteManager->loadSprite("alpha_guard.png", 0, 0, 64, 64);

						SGuardRotation *rotation = m_vGuardRotations[iCurrentRotationCount++];

						if (rotation->m_bConstant)
						{
							Guard *guard = m_xGameObjectManager->createGuard(sf::Vector2f(x * 64, y * 64), sf::Vector2f(64, 64), false, sprite, this);
							guard->setWatchSize(rotation->m_fSize);
							guard->setVisionRange(rotation->m_fRange);
							guard->setConstantRotation(rotation->m_bClockwise);
							m_vLevelObjects.push_back(guard);
						}
						else
						{
							Guard *guard = m_xGameObjectManager->createGuard(sf::Vector2f(x * 64, y * 64), sf::Vector2f(64, 64), false, sprite, this);
							guard->setWatchSize(rotation->m_fSize);
							guard->setVisionRange(rotation->m_fRange);
							guard->setRotationPoints(rotation->m_fMaxAngle, rotation->m_fMinAngle, rotation->m_bClockwise);
							m_vLevelObjects.push_back(guard);
						}

						break;
					}
				case esc::PATROLLINGGUARD:
					{
						sf::Sprite *sprite = m_xSpriteManager->loadSprite("alpha_guard.png", 0, 0, 64, 64);

						Guard *guard = m_xGameObjectManager->createGuard(sf::Vector2f(x * 64, y * 64), sf::Vector2f(64, 64), false, sprite, this);

						guard->setWatchSize(70);
						guard->setVisionRange(250);
						/*guard->setPatrolPath(m_vPatrolPaths[iCurrentGuardCount]);
						guard->setIsPatrolling(true);*/
						m_vLevelObjects.push_back(guard);
						vPatrollingGuards.push_back(guard);


						break;
					}
				case esc::DOOR:
					{

					}
				case esc::LOCKEDH:
					{
						Door *obj = m_xGameObjectManager->createDoor(sf::Vector2f(x * 64, y * 64), sf::Vector2f(64, 64), false, 1, EObjectType::LOCKEDH, m_xSpriteManager);
						m_vLevelObjects.push_back(obj);
						break;
					}
				case esc::LOCKEDV:
					{
						Door *obj = m_xGameObjectManager->createDoor(sf::Vector2f(x * 64, y * 64), sf::Vector2f(64, 64), true, 1, EObjectType::LOCKEDV, m_xSpriteManager);
						m_vLevelObjects.push_back(obj);
						break;
					}
				case esc::OPENH:
					{
						Door *obj = m_xGameObjectManager->createDoor(sf::Vector2f(x * 64, y * 64), sf::Vector2f(64, 64), false, 0, EObjectType::OPENH, m_xSpriteManager);
						m_vLevelObjects.push_back(obj);
						break;
					}
				case esc::OPENV:
					{
						Door *obj = m_xGameObjectManager->createDoor(sf::Vector2f(x * 64, y * 64), sf::Vector2f(64, 64), true, 0, EObjectType::OPENH, m_xSpriteManager);
						m_vLevelObjects.push_back(obj);
						break;
					}
				case esc::CLOSEDH:
					{
						Door *obj = m_xGameObjectManager->createDoor(sf::Vector2f(x * 64, y * 64), sf::Vector2f(64, 64), false, 2, EObjectType::CLOSEDH, m_xSpriteManager);
						m_vLevelObjects.push_back(obj);
						break;
					}
				case esc::CLOSEDV:
					{
						Door *obj = m_xGameObjectManager->createDoor(sf::Vector2f(x * 64, y * 64), sf::Vector2f(64, 64), true, 2, EObjectType::CLOSEDV, m_xSpriteManager);
						m_vLevelObjects.push_back(obj);
						break;
					}
				case esc::KEYCARD:
					{
						Item *card = m_xGameObjectManager->createItem(sf::Vector2f(x * 64, y * 64), sf::Vector2f(64, 64), EObjectType::KEYCARD, m_xSpriteManager->loadSprite("Keycard_pu.png", 0, 0, 64, 64), 0);
						m_vLevelObjects.push_back(card);
						break;
					}
				case esc::RECMID:
					{
						GameObject *obj = m_xGameObjectManager->createObject(sf::Vector2f(x * 64, y * 64), sf::Vector2f(64, 64), false, EObjectType::RECMID, m_xSpriteManager->loadSprite("Lobydesk2.png", 0, 0, 64, 64));
						m_vLevelObjects.push_back(obj);
						break;
					}
				case esc::RECTOP:
					{
						GameObject *obj = m_xGameObjectManager->createObject(sf::Vector2f(x * 64, y * 64), sf::Vector2f(64, 64), false, EObjectType::RECTOP, m_xSpriteManager->loadSprite("Lobydesk1.png", 0, 0, 64, 64));
						m_vLevelObjects.push_back(obj);
						break;
					}
				case esc::RECLOW:
					{
						GameObject *obj = m_xGameObjectManager->createObject(sf::Vector2f(x * 64, y * 64), sf::Vector2f(64, 64), false, EObjectType::RECLOW, m_xSpriteManager->loadSprite("Lobydesk3.png", 0, 0, 64, 64));
						m_vLevelObjects.push_back(obj);
						break;
					}
				case esc::FIKABORD:
					{
						GameObject *obj = m_xGameObjectManager->createObject(sf::Vector2f(x * 64, y * 64), sf::Vector2f(64, 64), false, EObjectType::WALL, m_xSpriteManager->loadSprite("coffe_table.png", 0, 0, 64, 64));
						m_vLevelObjects.push_back(obj);
						break;
					}
				case esc::STOLARUPP:
					{
						GameObject *obj = m_xGameObjectManager->createObject(sf::Vector2f(x * 64, y * 64), sf::Vector2f(64, 64), false, EObjectType::STOLARUPP, m_xSpriteManager->loadSprite("chairn.png", 0, 0, 64, 64));
						m_vLevelObjects.push_back(obj);
						break;
					}
				case esc::STOLARNER:
					{
						GameObject *obj = m_xGameObjectManager->createObject(sf::Vector2f(x * 64, y * 64), sf::Vector2f(64, 64), false, EObjectType::STOLARNER, m_xSpriteManager->loadSprite("chairu.png", 0, 0, 64, 64));
						m_vLevelObjects.push_back(obj);
						break;
					}
				case esc::STOLARV:
					{
						GameObject *obj = m_xGameObjectManager->createObject(sf::Vector2f(x * 64, y * 64), sf::Vector2f(64, 64), false, EObjectType::STOLARV, m_xSpriteManager->loadSprite("chairh.png", 0, 0, 64, 64));
						m_vLevelObjects.push_back(obj);
						break;
					}
				case esc::STOLARH:
					{
						GameObject *obj = m_xGameObjectManager->createObject(sf::Vector2f(x * 64, y * 64), sf::Vector2f(64, 64), false, EObjectType::STOLARH, m_xSpriteManager->loadSprite("chairv.png", 0, 0, 64, 64));
						m_vLevelObjects.push_back(obj);
						break;
					}
				case esc::OBJECTIVE:
					{
						Item *pda = m_xGameObjectManager->createItem(sf::Vector2f(x * 64, y * 64), sf::Vector2f(64, 64), EObjectType::OBJECTIVE, m_xSpriteManager->loadSprite("PDA.png", 0, 0, 64, 64), 0);
						m_vLevelObjects.push_back(pda);
						break;
					}
				case esc::GUN:
					{
						Item *gun = m_xGameObjectManager->createItem(sf::Vector2f(x * 64, y * 64), sf::Vector2f(64, 64), EObjectType::GUN, m_xSpriteManager->loadSprite("Gun_pu.png", 0, 0, 64, 64), 0);
						m_vLevelObjects.push_back(gun);
						break;
					}
				}

				vPathNodes.push_back(xNode);

			}

			m_vPathNodes.push_back(vPathNodes);


		}

		GameObject *tutobj1 = m_xGameObjectManager->createObject(sf::Vector2f(47 * 64, 7 * 64), sf::Vector2f(192, 192), false, EObjectType::TUTOBJ, m_xSpriteManager->loadSprite("WASD.png", 0, 0, 192, 192));
		m_vLevelObjects.push_back(tutobj1);
		m_vTutorialObjects.push_back(tutobj1);

		GameObject *tutobj2 = m_xGameObjectManager->createObject(sf::Vector2f(48 * 64, 11 * 64), sf::Vector2f(576, 64), false, EObjectType::TUTOBJ, m_xSpriteManager->loadSprite("SPACE.png", 0, 0, 576, 64));
		m_vLevelObjects.push_back(tutobj2);
		m_vTutorialObjects.push_back(tutobj2);

		GameObject *tutobj3 = m_xGameObjectManager->createObject(sf::Vector2f(59 * 64, 12 * 64), sf::Vector2f(64, 64), false, EObjectType::TUTOBJ, m_xSpriteManager->loadSprite("Einteract.png", 0, 0, 232, 64));
		m_vLevelObjects.push_back(tutobj3);
		m_vTutorialObjects.push_back(tutobj3);

		GameObject *tutobj4 = m_xGameObjectManager->createObject(sf::Vector2f(50 * 64, 12 * 64), sf::Vector2f(64, 64), false, EObjectType::TUTOBJ, m_xSpriteManager->loadSprite("Mouse.png", 0, 0, 64, 64));
		m_vLevelObjects.push_back(tutobj4);
		m_vTutorialObjects.push_back(tutobj4);

		GameObject *tutobj5 = m_xGameObjectManager->createObject(sf::Vector2f(64 * 63, 64 * 16), sf::Vector2f(64, 64), false, EObjectType::TUTOBJ, m_xSpriteManager->loadSprite("HideB.png", 0, 0, 64, 288));
		m_vLevelObjects.push_back(tutobj5);
		m_vTutorialObjects.push_back(tutobj5);

		GameObject *tutobj6 = m_xGameObjectManager->createObject(sf::Vector2f(64 * 20, 64 * 8), sf::Vector2f(64, 64), false, EObjectType::TUTOBJ, m_xSpriteManager->loadSprite("tut6.png", 0, 0, 64, 64));
		m_vLevelObjects.push_back(tutobj6);
		m_vTutorialObjects.push_back(tutobj6);

		for (auto object : m_vTutorialObjects)
		{
			object->setIsRemoved(true);
		}

		m_xPathFinder = new PathFind(this);

		getPatrolPaths(p_sPatrolPath);

		iCurrentGuardCount = 0;

		for (auto guard : vPatrollingGuards)
		{
			/*if (iCurrentGuardCount >= m_vPatrolPaths.size())
			break;*/
			guard->setPatrolPath(m_vPatrolPaths[iCurrentGuardCount]);
			guard->setIsPatrolling(true);
			iCurrentGuardCount++;

		}

		for (int y = 0; y < 200; y++)
		{
			for (int x = 0; x < 200; x++)
			{
				GameObject *obj = m_aAdjacantObjects[x][y];

				if (obj != nullptr)
				{
					bool upFree, downFree, leftFree, rightFree;

					if (m_aAdjacantObjects[x + 1][y] == nullptr || m_aAdjacantObjects[x + 1][y]->getType() == COUCH)
						rightFree = true;
					else
						rightFree = false;

					if (m_aAdjacantObjects[x - 1][y] == nullptr || m_aAdjacantObjects[x - 1][y]->getType() == COUCH)
						leftFree = true;
					else
						leftFree = false;

					if (m_aAdjacantObjects[x][y + 1] == nullptr || m_aAdjacantObjects[x][y + 1]->getType() == COUCH)
						downFree = true;
					else
						downFree = false;

					if (m_aAdjacantObjects[x][y - 1] == nullptr || m_aAdjacantObjects[x][y - 1]->getType() == COUCH)
						upFree = true;
					else
						upFree = false;

					if (obj->getType() == WALL)
					{
						if (!upFree && !downFree && !leftFree && !rightFree)
							obj->setSprite(m_xSpriteManager->loadSprite("Wall black.png", 0, 0, 64, 64));
						else if (upFree && downFree && !leftFree && rightFree)
							obj->setSprite(m_xSpriteManager->loadSprite("Wall 5.png", 0, 0, 64, 64));
						else if (upFree && !downFree && leftFree && rightFree)
							obj->setSprite(m_xSpriteManager->loadSprite("Wall 2.png", 0, 0, 64, 64));
						else if (upFree && downFree && leftFree && !rightFree)
							obj->setSprite(m_xSpriteManager->loadSprite("Wall 3.png", 0, 0, 64, 64));
						else if (!upFree && downFree && leftFree && rightFree)
							obj->setSprite(m_xSpriteManager->loadSprite("Wall 4.png", 0, 0, 64, 64));
						else if (upFree && downFree && !leftFree && rightFree)
							obj->setSprite(m_xSpriteManager->loadSprite("Wall 5.png", 0, 0, 64, 64));
						else if (!upFree && !downFree && leftFree && rightFree)
							obj->setSprite(m_xSpriteManager->loadSprite("Wall 6.png", 0, 0, 64, 64));
						else if (upFree && downFree && !leftFree && !rightFree)
							obj->setSprite(m_xSpriteManager->loadSprite("Wall 7.png", 0, 0, 64, 64));
						else if (upFree && !downFree && !leftFree && rightFree)
							obj->setSprite(m_xSpriteManager->loadSprite("Wall 8.png", 0, 0, 64, 64));
						else if (!upFree && downFree && !leftFree && rightFree)
							obj->setSprite(m_xSpriteManager->loadSprite("Wall 9.png", 0, 0, 64, 64));
						else if (upFree && !downFree && leftFree && !rightFree)
							obj->setSprite(m_xSpriteManager->loadSprite("Wall 10.png", 0, 0, 64, 64));
						else if (!upFree && downFree && leftFree && !rightFree)
							obj->setSprite(m_xSpriteManager->loadSprite("Wall 11.png", 0, 0, 64, 64));
						else if (!upFree && !downFree && leftFree && !rightFree)
							obj->setSprite(m_xSpriteManager->loadSprite("Wall 12.png", 0, 0, 64, 64));
						else if (!upFree && !downFree && !leftFree && rightFree)
							obj->setSprite(m_xSpriteManager->loadSprite("Wall 13.png", 0, 0, 64, 64));
						else if (!upFree && downFree && !leftFree && !rightFree)
							obj->setSprite(m_xSpriteManager->loadSprite("Wall 14.png", 0, 0, 64, 64));
						else if (upFree && !downFree && !leftFree && !rightFree)
							obj->setSprite(m_xSpriteManager->loadSprite("Wall 15.png", 0, 0, 64, 64));
						else
							obj->setSprite(m_xSpriteManager->loadSprite("Wall grey.png", 0, 0, 64, 64));
					}
					else if (obj->getType() == COUCH)
					{
						bool upCouch, downCouch, leftCouch, rightCouch, upFace, downFace, leftFace, rightFace;

						if (m_aAdjacantObjects[x + 1][y] != nullptr && m_aAdjacantObjects[x + 1][y]->getType() == COUCH)
							rightCouch = true;
						else
							rightCouch = false;

						if (m_aAdjacantObjects[x - 1][y] != nullptr && m_aAdjacantObjects[x - 1][y]->getType() == COUCH)
							leftCouch = true;
						else
							leftCouch = false;

						if (m_aAdjacantObjects[x][y + 1] != nullptr && m_aAdjacantObjects[x][y + 1]->getType() == COUCH)
							downCouch = true;
						else
							downCouch = false;

						if (m_aAdjacantObjects[x][y - 1] != nullptr && m_aAdjacantObjects[x][y - 1]->getType() == COUCH)
							upCouch = true;
						else
							upCouch = false;

						if (m_aAdjacantObjects[x + 1][y] != nullptr && m_aAdjacantObjects[x + 1][y]->getType() == WALL)
							leftFace = true;
						else
							leftFace = false;

						if (m_aAdjacantObjects[x - 1][y] != nullptr && m_aAdjacantObjects[x - 1][y]->getType() == WALL)
							rightFace = true;
						else
							rightFace = false;

						if (m_aAdjacantObjects[x][y + 1] != nullptr && m_aAdjacantObjects[x][y + 1]->getType() == WALL)
							upFace = true;
						else
							upFace = false;

						if (m_aAdjacantObjects[x][y - 1] != nullptr && m_aAdjacantObjects[x][y - 1]->getType() == WALL)
							downFace = true;
						else
							downFace = false;

						if (upCouch && rightFace)
							obj->setSprite(m_xSpriteManager->loadSprite("Couch_rotate4.png", 0, 0, 64, 64));
						if (upCouch && leftFace)
							obj->setSprite(m_xSpriteManager->loadSprite("Couch2_rotate2.png", 0, 0, 64, 64));
						if (downCouch && rightFace)
							obj->setSprite(m_xSpriteManager->loadSprite("Couch2_rotate4.png", 0, 0, 64, 64));
						if (downCouch && leftFace)
							obj->setSprite(m_xSpriteManager->loadSprite("Couch_rotate2.png", 0, 0, 64, 64));
						if (rightCouch && downFace)
							obj->setSprite(m_xSpriteManager->loadSprite("Couch_rotate1.png", 0, 0, 64, 64));
						if (rightCouch && upFace)
							obj->setSprite(m_xSpriteManager->loadSprite("Couch2_rotate3.png", 0, 0, 64, 64));
						if (leftCouch && downFace)
							obj->setSprite(m_xSpriteManager->loadSprite("Couch2_rotate1.png", 0, 0, 64, 64));
						if (leftCouch && upFace)
							obj->setSprite(m_xSpriteManager->loadSprite("Couch_rotate3.png", 0, 0, 64, 64));
					}

					else if (obj->getType() == DESKS)
					{
						bool upDesk, downDesk, leftDesk, rightDesk, upWall, downWall, leftWall, rightWall;

						if (m_aAdjacantObjects[x + 1][y] != nullptr && m_aAdjacantObjects[x + 1][y]->getType() == DESKS)
							rightDesk = true;
						else
							rightDesk = false;

						if (m_aAdjacantObjects[x - 1][y] != nullptr && m_aAdjacantObjects[x - 1][y]->getType() == DESKS)
							leftDesk = true;
						else
							leftDesk = false;

						if (m_aAdjacantObjects[x][y + 1] != nullptr && m_aAdjacantObjects[x][y + 1]->getType() == DESKS)
							downDesk = true;
						else
							downDesk = false;

						if (m_aAdjacantObjects[x][y - 1] != nullptr && m_aAdjacantObjects[x][y - 1]->getType() == DESKS)
							upDesk = true;
						else
							upDesk = false;


						if (m_aAdjacantObjects[x + 1][y] != nullptr && m_aAdjacantObjects[x + 1][y]->getType() == WALL)
							rightWall = true;
						else
							rightWall = false;

						if (m_aAdjacantObjects[x - 1][y] != nullptr && m_aAdjacantObjects[x - 1][y]->getType() == WALL)
							leftWall = true;
						else
							leftWall = false;

						if (m_aAdjacantObjects[x][y + 1] != nullptr && m_aAdjacantObjects[x][y + 1]->getType() == WALL)
							downWall = true;
						else
							downWall = false;

						if (m_aAdjacantObjects[x][y - 1] != nullptr && m_aAdjacantObjects[x][y - 1]->getType() == WALL)
							upWall = true;
						else
							upWall = false;

						if (upDesk && rightWall)
							obj->setSprite(m_xSpriteManager->loadSprite("desk2up.png", 0, 0, 64, 64));
						if (upDesk && leftWall)
							obj->setSprite(m_xSpriteManager->loadSprite("desk1up.png", 0, 0, 64, 64));
						if (downDesk && rightWall)
							obj->setSprite(m_xSpriteManager->loadSprite("desk1down.png", 0, 0, 64, 64));
						if (downDesk && leftWall)
							obj->setSprite(m_xSpriteManager->loadSprite("desk2down.png", 0, 0, 64, 64));
						if (leftDesk && upWall)
							obj->setSprite(m_xSpriteManager->loadSprite("desk2left.png", 0, 0, 64, 64));
						if (leftDesk && downWall)
							obj->setSprite(m_xSpriteManager->loadSprite("desk1left.png", 0, 0, 64, 64));
						if (rightDesk && upWall)
							obj->setSprite(m_xSpriteManager->loadSprite("desk1right.png", 0, 0, 64, 64));
						if (rightDesk && downWall)
							obj->setSprite(m_xSpriteManager->loadSprite("desk2right.png", 0, 0, 64, 64));
					}

				}
			}
		}

	}

	void Level::createFloor(std::string p_sFilePath)
	{
		sf::Image xImage;

		xImage.loadFromFile(m_sDirectoryPath + p_sFilePath);

		int iCurrentGuardCount = 0;

		int iCurrentRotationCount = 0;

		for (int y = 0; y < xImage.getSize().y; y++)
		{
			for (int x = 0; x < xImage.getSize().x; x++)
			{
				sf::Color currentColor = xImage.getPixel(x, y);

				sf::Color *colorPointer = nullptr;

				for (auto color : m_vFloorColorCodes)
				{
					if (currentColor == *color)
					{
						colorPointer = color;
						break;
					}

				}

				auto iterCurrentObjectType = m_mFloorColorCodes.find(colorPointer);

				if (iterCurrentObjectType == m_mFloorColorCodes.end())
				{
					continue;
				}

				switch (iterCurrentObjectType->second)
				{
				case EFloorType::NORMAL:
					{
						sf::Sprite *sprite = m_xSpriteManager->loadSprite("Floor1.png", 0, 0, 64, 64);
						sprite->setOrigin(32, 32);
						sprite->setPosition(sf::Vector2f(x * 64, y * 64));
						m_vFloorSprites.push_back(sprite);
						break;
					}

				case EFloorType::NORMALFLOOR:
					{
						sf::Sprite *sprite = m_xSpriteManager->loadSprite("Floor5.png", 0, 0, 64, 64);
						sprite->setOrigin(32, 32);
						sprite->setPosition(sf::Vector2f(x * 64, y * 64));
						m_vFloorSprites.push_back(sprite);
						break;
					}
				case EFloorType::FLOOR2:
				{
												sf::Sprite *sprite = m_xSpriteManager->loadSprite("Floor2.png", 0, 0, 64, 64);
												sprite->setOrigin(32, 32);
												sprite->setPosition(sf::Vector2f(x * 64, y * 64));
												m_vFloorSprites.push_back(sprite);
												break;
				}
				case EFloorType::FLOOR3:
				{
												sf::Sprite *sprite = m_xSpriteManager->loadSprite("Floor3.png", 0, 0, 64, 64);
												sprite->setOrigin(32, 32);
												sprite->setPosition(sf::Vector2f(x * 64, y * 64));
												m_vFloorSprites.push_back(sprite);
												break;
				}
				case EFloorType::FLOOR4:
				{
												sf::Sprite *sprite = m_xSpriteManager->loadSprite("Floor4.png", 0, 0, 64, 64);
												sprite->setOrigin(32, 32);
												sprite->setPosition(sf::Vector2f(x * 64, y * 64));
												m_vFloorSprites.push_back(sprite);
												break;
				}
				}

			}
		}
	}

	void Level::getPatrolPaths(std::string p_sFilePath)
	{
		std::fstream stream(m_sDirectoryPath + p_sFilePath);

		std::vector<std::vector<sf::Vector2f*>> vAllPatrolPaths;

		while (!stream.eof())
		{
			stream >> m_iGuardCount;

			std::vector<sf::Vector2f*> vPatrolPaths;

			for (int i = 0; i < m_iGuardCount; i++)
			{
				int iPointsCount;

				stream >> iPointsCount;

				for (int z = 0; z < iPointsCount; z++)
				{
					int x, y;

					stream >> x >> y;

					vPatrolPaths.push_back(new sf::Vector2f(x * 64 + 1, y * 64 + 1));
				}

				std::vector<sf::Vector2f*> vAllPatrolPoints;

				for (int i = 0; i < vPatrolPaths.size(); i++)
				{
					if (i < vPatrolPaths.size() - 1)
					{
						std::vector<sf::Vector2f*> vCurrentPath = m_xPathFinder->pathToPosition(vPatrolPaths[i + 1], vPatrolPaths[i]);
						vAllPatrolPoints.insert(vAllPatrolPoints.end(), vCurrentPath.begin(), vCurrentPath.end());
					}
					else
					{
						std::vector<sf::Vector2f*> vCurrentPath = m_xPathFinder->pathToPosition(vPatrolPaths[0], vPatrolPaths[i]);
						vAllPatrolPoints.insert(vAllPatrolPoints.end(), vCurrentPath.begin(), vCurrentPath.end());
					}
				}

				vAllPatrolPaths.push_back(vAllPatrolPoints);

				vPatrolPaths.clear();
			}


		}

		m_vPatrolPaths = vAllPatrolPaths;
	}

	void Level::getRotationsPaths(std::string p_sFilePath)
	{
		std::fstream stream(m_sDirectoryPath + p_sFilePath);

		std::vector<SGuardRotation*> vAllRotations;

		while (!stream.eof())
		{
			stream >> m_iRotateCount;

			for (int i = 0; i < m_iRotateCount; i++)
			{
				int iCheckConstant;

				int iCheckClockWise;

				float fRange, fSize;

				stream >> iCheckConstant;

				stream >> iCheckClockWise;

				stream >> fSize;

				stream >> fRange;

				SGuardRotation *rotation = new SGuardRotation;

				rotation->m_fRange = fRange;
				rotation->m_fSize = fSize;

				if (iCheckConstant == 0)
				{

					rotation->m_bConstant = true;

					if (iCheckClockWise == 0)
					{
						rotation->m_bClockwise = true;
					}
					else
					{
						rotation->m_bClockwise = false;
					}

					vAllRotations.push_back(rotation);
				}
				else
				{
					rotation->m_bConstant = false;

					stream >> rotation->m_fMaxAngle;
					stream >> rotation->m_fMinAngle;

					vAllRotations.push_back(rotation);
				}
			}
		}

		m_vGuardRotations = vAllRotations;
	}

	void Level::reset()
	{
		for (auto object : m_vLevelObjects)
		{
			if (object->getType() == PATROLLINGGUARD || object->getType() == STATIONARYGUARD)
			{
				static_cast<Guard*>(object)->reset();
			}
			else if (object->getType() == BATON || object->getType() == PDA || object->getType() == KEYCARD)
			{
				object->setIsRemoved(false);
			}
			else if (object->getType() == KEYCARD || object->getType() == PDA)
			{
				object->setIsRemoved(false);
			}
			else if (object->getType() == BATON)
			{
				object->setIsRemoved(true);
			}

		}
	}

	void Level::destroy()
	{

	}

	void Level::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{

		if (m_vFloorSprites.size() > 0)
		{
			for (auto sprite : m_vFloorSprites)
			{
				target.draw(*sprite, states);
			}
		}

		/*for (auto object : m_vLevelObjects)
		{
		if (object->getType() != PATROLLINGGUARD && object->getType() != STATIONARYGUARD)
		{
		target.draw(*object, states);
		}
		}

		for (auto object : m_vLevelObjects)
		{
		if (object->getType() == PATROLLINGGUARD || object->getType() == STATIONARYGUARD)
		{
		target.draw(*object, states);
		}
		}*/
	}

	void Level::setStateNum(int p_statenum)
	{
		m_iStateNum = p_statenum;
	}
	int Level::getStateNum()
	{
		return m_iStateNum;
	}

	std::vector<GameObject*> *Level::getObjects()
	{
		return &m_vLevelObjects;
	}

	std::vector<GameObject*> *Level::getTutObjects()
	{
		return &m_vTutorialObjects;
	}

	std::vector<std::vector<PathNode*>> *Level::getPathNodes()
	{
		return &m_vPathNodes;
	}

	SpriteManager* Level::getSpriteManager()
	{
		return m_xSpriteManager;
	}


	void Level::handleTutObjects(int p_iObjNum, bool p_bAddOrRemove)
	{
		GameObject* obj = m_vTutorialObjects[p_iObjNum - 1];
		if (p_bAddOrRemove)
			obj->setIsRemoved(false);
		else
			obj->setIsRemoved(true);
	}

	void Level::getWalls(GameObject *p_aWalls[200][200])
	{
		for (int y = 0; y < 200; y++)
		{
			for (int x = 0; x < 200; x++)
			{
				p_aWalls[x][y] = m_aWalls[x][y];
			}
		}

	}
}
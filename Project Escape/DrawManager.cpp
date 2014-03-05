#include "DrawManager.h"
#include "GameObject.h"
#include "Level.h"

namespace esc
{

	DrawManager::DrawManager(sf::RenderWindow *window)
	{
		m_window = window;
	}

	void DrawManager::draw(std::vector<esc::GameObject*> *objects)
	{
		for (auto object : *objects)
		{
			m_window->draw(*object);
		}
	}

	void DrawManager::draw(Level *level)
	{
		m_window->draw(*level);
	}

}
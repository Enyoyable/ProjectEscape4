#pragma once

#include "stdafx.h"

namespace esc
{

	class GameObject;
	class Level;

	class DrawManager
	{
	public:
		DrawManager(sf::RenderWindow *window);

		void draw(std::vector<GameObject*> *objects);

		void draw(Level *level);

	private:
		sf::RenderWindow *m_window;
	};

}
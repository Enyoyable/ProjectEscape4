#pragma once

#include "stdafx.h"

namespace esc
{

	class Engine
	{
	public:
		Engine();
		~Engine();

		void initialize();
		void run();
		void cleanup();

		sf::RenderWindow *m_window;
	};

}
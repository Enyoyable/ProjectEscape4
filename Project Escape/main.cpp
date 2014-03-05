#include "Engine.h"

int main()
{
	esc::Engine engine;
	engine.initialize();
	engine.run();
	engine.cleanup();

	return 0;
}
#include "stdio.h"
#include "gamecore.h"
#include <GL/glew.h>
#include <string>
#include <fstream>
#include <streambuf>

const int MS_PER_UPDATE = 16;

int main(int argc, char** argv)
{
	GameCore gameCore;

	gameCore.init(800, 600);

	double previous = gameCore.getElapsedTime();
	double lag = 0.0;
	while (gameCore.update())
	{
		double current = gameCore.getElapsedTime();
		double elapsed = current - previous;
		previous = current;
		lag += elapsed;

		gameCore.processInput();

		while (lag >= MS_PER_UPDATE)
		{
			gameCore.update();
			lag -= MS_PER_UPDATE;
		}

		gameCore.render();

	}
	gameCore.destroy();
	return 0;
}
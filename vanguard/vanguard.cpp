#include "stdio.h"
#include "gamecore.h"
#include "SpriteRenderer.h"
#include <GL/glew.h>
#include <string>
#include <fstream>
#include <streambuf>
#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include "ParticleSystem.h"
const int MS_PER_UPDATE = 16;


int main(int argc, char** argv)
{
	GameCore gameCore;

	gameCore.init(1280, 720);

	double previous = gameCore.getElapsedTime();
	double lag = 0.0;

	//ParticleSystem particleSystem;
	//particleSystem.Initialize();

	SpriteRenderer sprite;
	sprite.Initialize("megaman.jpg");

	while (gameCore.update())
	{
		double current = gameCore.getElapsedTime();
		double elapsed = current - previous;
		previous = current;
		lag += elapsed;

		gameCore.processInput();
		//particleSystem.Update();
		while (lag >= MS_PER_UPDATE)
		{
			gameCore.update();
			//particleSystem.Update(current);
			lag -= MS_PER_UPDATE;
		}
		//printf("Elapsed time: %f\n", elapsed);
		//particleSystem.Render();
		sprite.Render(glm::vec2(200.0f, 100.0f), glm::vec2(200, 200));
		gameCore.render();

	}
	gameCore.destroy();
	return 0;
}
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

	ParticleSystem particleSystem;
	particleSystem.Initialize();
	ParticleSystem particleSystem2;
	particleSystem2.Initialize();
	ParticleSystem particleSystem3;
	particleSystem3.Initialize();
	ParticleSystem particleSystem4;
	particleSystem4.Initialize();

	SpriteRenderer sprite;
	sprite.Initialize("megaman.jpg");

	while (gameCore.update())
	{
		double current = gameCore.getElapsedTime();
		double elapsed = current - previous;
		previous = current;
		lag += elapsed;

		gameCore.processInput();
		particleSystem.Update();
		particleSystem2.Update();
		//particleSystem3.Update();
		//particleSystem4.Update();
		while (lag >= MS_PER_UPDATE)
		{
			gameCore.update();
			//particleSystem.Update(current);
			lag -= MS_PER_UPDATE;
		}
		//printf("Elapsed time: %f\n", elapsed);
		
		particleSystem.Render();
		particleSystem2.Render();
		//particleSystem3.Render();
		//particleSystem4.Render();
		//sprite.Render(glm::vec2(200.0f, 100.0f), glm::vec2(200, 200));
		gameCore.render();
	}
	gameCore.destroy();
	return 0;
}
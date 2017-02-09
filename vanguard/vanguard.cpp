#include "stdio.h"
#include "gamecore.h"
#include <GL/glew.h>
#include <string>
#include <fstream>
#include <streambuf>
#include <glm/glm.hpp>
#include <vector>
#include <iostream>
const int MS_PER_UPDATE = 16;


int main(int argc, char** argv)
{
	GameCore gameCore;

	gameCore.init(800, 600);

	GLuint vertexArrayObject;
	GLuint shaderProgram;

	GLuint positionBuffer;
	glGenBuffers(1, &positionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * 2 * 10000, nullptr, GL_DYNAMIC_DRAW);

	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
	glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(0);

	shaderProgram = gameCore.loadShaderProgram("particle.vert", "particle.frag");

	std::vector<glm::vec2> data;
	data.resize(10000);
	for (int i = 0; i < 10000; ++i) {
		data[i] = glm::vec2(gameCore.randf() * 2 - 1, gameCore.randf() * 2 - 1);
	}

	glBindVertexArray(vertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * 2 * 10000, data.data(), GL_DYNAMIC_DRAW);
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

		gameCore.render(shaderProgram, vertexArrayObject);

	}
	gameCore.destroy();
	return 0;
}
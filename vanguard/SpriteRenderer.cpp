#include "SpriteRenderer.h"
#include <stb_image.h>
#include "gamecore.h"

void SpriteRenderer::Initialize()
{
	CreateVAO();
	spriteShader = GameCore::loadShaderProgram("sprite.vert", "sprite.frag");
}

void SpriteRenderer::Render(glm::vec2 position) 
{
	glUseProgram(spriteShader);
	glBindVertexArray(vao);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void SpriteRenderer::CreateVAO()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	const glm::vec2 positions[] = {
		{ -0.5f, -0.5f },
		{ 0.5f, -0.5f },
		{ 0.5f, 0.5f },
		{ -0.5f, 0.5f },
	};
	glGenBuffers(1, &vbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, 0);

	float texcoords[] = {
		0.0f, 0.0f, // (u,v) for v0 
		0.0f, 1.0f, // (u,v) for v1
		1.0f, 1.0f, // (u,v) for v2
		1.0f, 0.0f // (u,v) for v3
	};
	glGenBuffers(1, &texbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, texbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texcoords), texcoords, GL_DYNAMIC_DRAW);

	const int indices[] = {
		0, 1, 3, // Triangle 1
		2, 3, 1  // Triangle 2
	};
	glGenBuffers(1, &ibuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(1, 2, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
}
#include "SpriteRenderer.h"
#include "gamecore.h"
#include <glm/gtc/matrix_transform.hpp>

void SpriteRenderer::Initialize(const char* spriteFile, TextureFormat format,
	TextureFiltering filtering)
{
	CreateVAO();
	texture = GameCore::LoadTexture(spriteFile, format, filtering);

	spriteShader = GameCore::loadShaderProgram("sprite.vert", "sprite.frag");
	glUseProgram(spriteShader);
	glm::mat4 projection = glm::ortho(0.0f, 1280.0f, 720.0f, 0.0f, -1.0f, 1.0f);
	GameCore::setUniform(spriteShader, "projection", projection);
}

void SpriteRenderer::Render(glm::vec2 position, glm::vec2 size) 
{
	glUseProgram(spriteShader);
	glBindVertexArray(vao);

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(position, 0.0f));
	model = glm::scale(model, glm::vec3(size, 1.0f));
	GameCore::setUniform(spriteShader, "model", model);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void SpriteRenderer::CreateVAO()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	const glm::vec2 positions[] = {
		{ 0.0f, 0.0f },
		{ 1.0f, 0.0f },
		{ 1.0f, 1.0f },
		{ 0.0f, 1.0f },
	};
	glGenBuffers(1, &vbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, 0);

	float texcoords[] = {
		0.0f, 0.0f,
		1.0f, 0.0f, 
		1.0f, 1.0f,
		0.0f, 1.0f
	};
	glGenBuffers(1, &texbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, texbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texcoords), texcoords, GL_DYNAMIC_DRAW);

	const int indices[] = {
		3, 1, 0,
		3, 2, 1
	};
	glGenBuffers(1, &ibuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(1, 2, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
}
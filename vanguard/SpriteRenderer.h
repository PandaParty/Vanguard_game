#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
class SpriteRenderer
{
public:
	void Initialize();
	void Render(glm::vec2);
private:
	GLuint vbuffer, ibuffer, texbuffer, vao, texture, spriteShader;

	void CreateVAO();

};
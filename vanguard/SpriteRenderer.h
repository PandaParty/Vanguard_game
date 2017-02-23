#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "gamecore.h"

class SpriteRenderer
{
public:
	void Initialize(const char* spriteFile, TextureFormat format = TextureFormat::RGBA,
		TextureFiltering filtering = TextureFiltering::TRILINEAR);
	void Render(glm::vec2 position, glm::vec2 size);
private:
	GLuint vbuffer, ibuffer, texbuffer, vao, texture, spriteShader;

	void CreateVAO();

};
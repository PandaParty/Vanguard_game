#include "SpriteRenderer.h"
#include "gamecore.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glm/gtc/matrix_transform.hpp>

void SpriteRenderer::Initialize(const char* spriteFile, TextureFormat format,
	TextureFiltering filtering)
{
	CreateVAO();
	LoadTexture(spriteFile, format, filtering);

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

void SpriteRenderer::LoadTexture(const char* spriteFile, TextureFormat format, TextureFiltering filtering)
{
	int w, h, comp;
	unsigned char* image = stbi_load(spriteFile, &w, &h, &comp, STBI_rgb_alpha);

	GLint internalFormat;
	switch (format)
	{
	case GRAY:
		internalFormat = GL_RED;
		break;
	case RGB:
		internalFormat = GL_RGB;
		break;
	case RGBA:
	default:
		internalFormat = GL_RGBA;
	}

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, w, h, 0, internalFormat, GL_UNSIGNED_BYTE, image);
	free(image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	switch (filtering)
	{
	case NEAREST:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		break; 
	case BILINEAR:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		break;
	case TRILINEAR:
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		break;
	case ANISOTROPIC_1:
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1.0f);
		break;
	case ANISOTROPIC_2:
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 2.0f);
		break;
	case ANISOTROPIC_4:
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4.0f);
		break;
	case ANISOTROPIC_8:
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 8.0f);
		break;
	case ANISOTROPIC_16:
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);
		break;
	}

	

	
}
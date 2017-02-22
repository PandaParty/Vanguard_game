#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
class SpriteRenderer
{
	enum TextureFiltering
	{
		NEAREST,
		BILINEAR,
		TRILINEAR,
		ANISOTROPIC_1,
		ANISOTROPIC_2,
		ANISOTROPIC_4,
		ANISOTROPIC_8,
		ANISOTROPIC_16
	};

	enum TextureFormat
	{
		GRAY,
		RGB,
		RGBA
	};
public:
	void Initialize(const char* spriteFile, TextureFormat format = TextureFormat::RGBA,
		TextureFiltering filtering = TextureFiltering::TRILINEAR);
	void Render(glm::vec2 position, glm::vec2 size);
private:
	GLuint vbuffer, ibuffer, texbuffer, vao, texture, spriteShader;

	void CreateVAO();
	void LoadTexture(const char* spriteFile, TextureFormat format, TextureFiltering filtering);

};
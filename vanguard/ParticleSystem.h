#pragma once
#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>

class ParticleSystem
{
public:
	void Initialize();
	void Update(float elapsedTime);
	void Render();

private:
	GLuint vertexArrayObject;
	GLuint particleBuffer;
	GLuint shaderProgram;
	void CreateVertexArrayObject();
	int firstActiveParticle;
	int firstDeadParticle;
	std::vector<glm::vec2> data;
	int maxParticles;
	int spawnRate;
	float lifeTime;
};
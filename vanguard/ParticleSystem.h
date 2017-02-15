#pragma once
#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>

class ParticleSystem
{
public:
	void Initialize();
	void Update();
	void Render();

private:
	GLuint vertexArrayObject;
	GLuint particleBuffer;
	GLuint timeBuffer;
	GLuint forceBuffer;
	GLuint shaderProgram;
	int firstActiveParticle;
	int firstFreeParticle;
	std::vector<glm::vec4> data;
	std::vector<float> timeData;
	std::vector<glm::vec2> forceData;

	int maxParticles;
	int spawnRate;
	int lifeTime;
	int currentFrames;
	glm::vec2 emitterSize;
	glm::vec2 position;
	glm::vec2 direction;
	glm::vec2 force;
	float speed;
	float spread;
	float outwardVelocity;

	void CreateVertexArrayObject();
	void Spawn();
	glm::vec2 CalculateSpread(const glm::vec2 particleDir);
};
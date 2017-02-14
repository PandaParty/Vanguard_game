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
	GLuint shaderProgram;
	void CreateVertexArrayObject();
	int firstActiveParticle;
	int firstFreeParticle;
	
	std::vector<glm::vec4> data;
	std::vector<float> timeData;
	int maxParticles;
	int spawnRate;
	int lifeTime;
	int currentFrames;

	void Spawn();
};
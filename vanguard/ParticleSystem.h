#pragma once
#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

class ParticleSystem
{
	enum Shape {
		Rectangle,
		Circle
	};

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
	std::vector<glm::vec2> timeData;
	std::vector<glm::vec3> forceData;

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
	float friction;
	bool oneShot;
	Shape shape;

	void CreateVertexArrayObject();
	void Spawn();
	glm::vec2 CalculateSpread(const glm::vec2 particleDir);

	
};
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
	void Initialize(const char* textureFile);
	void Update();
	void Render();

private:
	GLuint vertexArrayObject;
	GLuint particleBuffer;
	GLuint timeBuffer;
	GLuint forceBuffer;
	GLuint sizeBuffer;
	GLuint shaderProgram;
	int firstActiveParticle;
	int firstFreeParticle;
	int currentFrames;
	std::vector<glm::vec4> data;
	std::vector<glm::vec2> timeData;
	std::vector<glm::vec3> forceData;
	std::vector<float> sizeData;
	GLuint texture;

	int maxParticles;
	int spawnRate;
	int lifeTime;
	float minSize;
	float maxSize;
	glm::vec2 emitterSize;
	glm::vec2 position;
	glm::vec2 direction;
	glm::vec2 force;
	float minSpeed;
	float maxSpeed;
	float spread;
	float outwardVelocity;
	float friction;
	bool oneShot;
	Shape shape;


	glm::vec2 emitterDirection;
	float emitterSpeed;

	glm::vec3 color;

	void CreateVertexArrayObject();
	void Spawn();
	glm::vec2 CalculateSpread(const glm::vec2 particleDir);
};
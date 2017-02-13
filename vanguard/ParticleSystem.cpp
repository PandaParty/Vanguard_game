#include "ParticleSystem.h"
#include "gamecore.h"

void ParticleSystem::Initialize()
{
	maxParticles = 10000;
	spawnRate = 1;
	lifeTime = 1.0f;
	firstActiveParticle = 0;
	firstDeadParticle = 0;
	CreateVertexArrayObject();
	shaderProgram = GameCore::loadShaderProgram("particle.vert", "particle.frag");

	data.resize(spawnRate);
	
	glBindVertexArray(vertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, particleBuffer);
	//glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec2) * maxParticles, data.data());
}

void ParticleSystem::Update(float elapsedTime)
{
	for (int i = 0; i < spawnRate; ++i) {
		data[i] = glm::vec2(0.0f, 0.0f);
	}

	firstDeadParticle += spawnRate;
	glBindVertexArray(vertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, particleBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, firstActiveParticle, sizeof(glm::vec2) * firstDeadParticle, data.data());
}

void ParticleSystem::Render()
{
	glUseProgram(shaderProgram);
	glBindVertexArray(vertexArrayObject);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glDrawArrays(GL_POINTS, firstActiveParticle, firstDeadParticle);
}

void ParticleSystem::CreateVertexArrayObject() 
{
	glGenBuffers(1, &particleBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, particleBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * maxParticles, nullptr, GL_DYNAMIC_DRAW);

	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, particleBuffer);
	glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(0);
}
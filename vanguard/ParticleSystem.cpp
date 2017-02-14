#include "ParticleSystem.h"
#include "gamecore.h"

void ParticleSystem::Initialize()
{
	maxParticles = 10000000;
	spawnRate = 10000;
	lifeTime = 20000;
	currentFrames = 0;
	firstActiveParticle = 0;
	firstDeadParticle = 0;
	CreateVertexArrayObject();
	shaderProgram = GameCore::loadShaderProgram("particle.vert", "particle.frag");

	data.resize(spawnRate);
	timeData.resize(spawnRate);
}

void ParticleSystem::Update(float elapsedTime)
{
	GameCore::setUniform(shaderProgram, "time", elapsedTime);
	if (++currentFrames > lifeTime)
	{
		firstActiveParticle += spawnRate;
	}
	for (int i = 0; i < spawnRate; ++i) {
		data[i] = glm::vec4(0.0f, 0.0f, (GameCore::randf() * 2 - 1) / 10000, (GameCore::randf() * 2 - 1) / 10000);
		timeData[i] = elapsedTime;
	}

	firstDeadParticle += spawnRate;
	glBindVertexArray(vertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, particleBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * (firstDeadParticle - spawnRate), sizeof(glm::vec4) * spawnRate, data.data());

	glBindBuffer(GL_ARRAY_BUFFER, timeBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * (firstDeadParticle - spawnRate), sizeof(GL_FLOAT) * spawnRate, timeData.data());

	if (firstDeadParticle > maxParticles)
	{
		printf("reached max particles");
	}

}

void ParticleSystem::Render()
{
	glUseProgram(shaderProgram);
	glBindVertexArray(vertexArrayObject);
	glEnable(GL_PROGRAM_POINT_SIZE);
	if (firstActiveParticle < firstDeadParticle)
	{
		glDrawArrays(GL_POINTS, firstActiveParticle, firstDeadParticle);
	}
}

void ParticleSystem::CreateVertexArrayObject() 
{
	glGenBuffers(1, &particleBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, particleBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * maxParticles, nullptr, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &timeBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, timeBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * maxParticles, nullptr, GL_DYNAMIC_DRAW);

	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, particleBuffer);
	glVertexAttribPointer(0, 4, GL_FLOAT, false, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, timeBuffer);
	glVertexAttribPointer(1, 1, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
}
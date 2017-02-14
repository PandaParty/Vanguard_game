#include "ParticleSystem.h"
#include "gamecore.h"

void ParticleSystem::Initialize()
{
	maxParticles = 100000000;
	spawnRate = 1000;
	lifeTime = 10000;
	currentFrames = 0;
	firstActiveParticle = 0;
	firstFreeParticle = 0;
	CreateVertexArrayObject();
	shaderProgram = GameCore::loadShaderProgram("particle.vert", "particle.frag");

	data.resize(spawnRate);
	timeData.resize(spawnRate);

	Spawn();
}

void ParticleSystem::Update()
{
	bool shouldSpawn = false;
	GameCore::setUniform(shaderProgram, "time", (float)currentFrames);

	if (++currentFrames > lifeTime)
	{
		firstActiveParticle += spawnRate;
	}
	if (firstFreeParticle >= maxParticles && firstActiveParticle != 0)
	{
		firstFreeParticle = 0;
	}
	if (firstActiveParticle >= maxParticles)
	{
		firstActiveParticle = 0;
	}

	if(firstActiveParticle < firstFreeParticle)
	{
		shouldSpawn = firstFreeParticle - firstActiveParticle < maxParticles;
	}
	else if(firstActiveParticle - firstFreeParticle > 0)
	{
		shouldSpawn = true;
	}

	if (shouldSpawn) 
	{
		Spawn();
	}

	
}

void ParticleSystem::Spawn()
{
	for (int i = 0; i < spawnRate; ++i) {
		data[i] = glm::vec4(0.0f, 0.0f, (GameCore::randf() * 2 - 1) / 500, (GameCore::randf() * 2 - 1) / 500);
		timeData[i] = (float)currentFrames;
	}

	firstFreeParticle += spawnRate;
	glBindVertexArray(vertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, particleBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * (firstFreeParticle - spawnRate), sizeof(glm::vec4) * spawnRate, data.data());

	glBindBuffer(GL_ARRAY_BUFFER, timeBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * (firstFreeParticle - spawnRate), sizeof(GL_FLOAT) * spawnRate, timeData.data());

}

void ParticleSystem::Render()
{
	glUseProgram(shaderProgram);
	glBindVertexArray(vertexArrayObject);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	if (firstActiveParticle < firstFreeParticle)
	{
		glDrawArrays(GL_POINTS, firstActiveParticle, firstFreeParticle - firstActiveParticle);
	}
	else if (firstActiveParticle > 0)
	{
	    glDrawArrays(GL_POINTS, firstActiveParticle, maxParticles - firstActiveParticle);
		if (firstFreeParticle > 0)
		{
			glDrawArrays(GL_POINTS, 0, firstFreeParticle);
		}
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
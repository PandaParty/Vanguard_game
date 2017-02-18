#include "ParticleSystem.h"
#include "gamecore.h"
#include <algorithm>

void ParticleSystem::Initialize()
{
	spawnRate = 1000;
	lifeTime = 100;
	maxParticles = std::min(spawnRate * lifeTime, 1000000);
	emitterSize = glm::vec2(0.1f, 0.1f);
	position = glm::vec2(0.0f, 0.0f);
	direction = glm::vec2(-0.0f, 0.0f);
	force = glm::vec2(0.0000f, 0.0000f);
	speed = 0.02f;
	spread = 0.0f;
	outwardVelocity = 1.0f;
	friction = 0.97f;
	shape = Circle;
	oneShot = true;

	currentFrames = 0;
	firstActiveParticle = 0;
	firstFreeParticle = 0;

	CreateVertexArrayObject();
	shaderProgram = GameCore::loadShaderProgram("particle.vert", "particle.frag");
	data.resize(spawnRate);
	timeData.resize(spawnRate);
	forceData.resize(spawnRate);
	if (oneShot)
	{
		firstFreeParticle = spawnRate;
	}

	Spawn();
}

void ParticleSystem::Update()
{
	bool shouldSpawn = false;
	GameCore::setUniform(shaderProgram, "time", (float)currentFrames);

	if (++currentFrames > lifeTime)
	{
		if (oneShot)
		{
			shouldSpawn = true;
			currentFrames = 0;
		}
		else
		{
			firstActiveParticle += spawnRate;
		}
	}
	if (!oneShot)
	{
		if (firstFreeParticle >= maxParticles && firstActiveParticle != 0)
		{
			firstFreeParticle = 0;
		}
		if (firstActiveParticle >= maxParticles)
		{
			firstActiveParticle = 0;
		}

		if (firstActiveParticle < firstFreeParticle)
		{
			shouldSpawn = firstFreeParticle - firstActiveParticle < maxParticles;
		}
		else if (firstActiveParticle - firstFreeParticle > 0)
		{
			shouldSpawn = true;
		}
	}
	
	if (shouldSpawn) 
	{
		Spawn();
	}

	
}

void ParticleSystem::Spawn()
{
	for (int i = 0; i < spawnRate; ++i) {
		glm::vec2 particlePos;
		if (shape == Rectangle)
		{
			particlePos = glm::vec2(position.x + GameCore::randf() * emitterSize.x - emitterSize.x / 2,
				position.y + GameCore::randf() * emitterSize.y - emitterSize.y / 2);
		}
		else
		{
			float angle = GameCore::randf() * glm::pi<float>() * 2.0f;
			float r = sqrt(GameCore::randf());
			particlePos = glm::vec2(position.x + r * cos(angle) * emitterSize.x / 2,
				position.y + r * sin(angle) * emitterSize.x / 2);
		}
		
		glm::vec2 particleDir = glm::vec2(direction.x, direction.y);
		particleDir += CalculateSpread(particleDir);
		if (particleDir != glm::vec2(0, 0))
		{
			particleDir = glm::normalize(particleDir);
		}
		particleDir += glm::normalize(particlePos - position) * outwardVelocity;
		data[i] = glm::vec4(particlePos.x, 
			particlePos.y,
			particleDir.x * speed,
			particleDir.y * speed);
		timeData[i] = glm::vec2((float)currentFrames, (float)lifeTime);
		forceData[i] = glm::vec3(force, friction);
	}
	if (!oneShot)
	{
		firstFreeParticle += spawnRate;
	}
	glBindVertexArray(vertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, particleBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * (firstFreeParticle - spawnRate), sizeof(glm::vec4) * spawnRate, data.data());

	glBindBuffer(GL_ARRAY_BUFFER, timeBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * (firstFreeParticle - spawnRate), sizeof(glm::vec2) * spawnRate, timeData.data());

	glBindBuffer(GL_ARRAY_BUFFER, forceBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * (firstFreeParticle - spawnRate), sizeof(glm::vec3) * spawnRate, forceData.data());
}

void ParticleSystem::Render()
{
	glUseProgram(shaderProgram);
	glBindVertexArray(vertexArrayObject);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

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
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * maxParticles, nullptr, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &forceBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, forceBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * maxParticles, nullptr, GL_DYNAMIC_DRAW);

	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, particleBuffer);
	glVertexAttribPointer(0, 4, GL_FLOAT, false, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, timeBuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, false, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, forceBuffer);
	glVertexAttribPointer(2, 3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
}

glm::vec2 ParticleSystem::CalculateSpread(const glm::vec2 particleDir)
{
	if (spread == 0.0f || (particleDir.x == 0.0f && particleDir.y == 0.0f))
		return glm::vec2(0.0f);
	float rotation = glm::acos(glm::dot(glm::vec2(0.0f, -1.0f), particleDir));
	if (glm::dot(glm::vec2(0.0f, -1.0f), glm::vec2(particleDir.y, -particleDir.x)) < 0.0f)
	{
		rotation *= -1;
	}
	float randomNumber = GameCore::randf() * 2 - 1;
	return glm::vec2(glm::sin(rotation - glm::radians(randomNumber * spread)),
		-glm::cos(rotation - glm::radians(randomNumber * spread)));
}
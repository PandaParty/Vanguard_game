#version 420
layout(location = 0) in vec4 particle;
layout(location = 1) in float startTime;
layout(location = 2) in vec2 force;

uniform float time;

void main()
{
	float lifeTime = time - startTime;
	vec2 position = vec2(particle.x + particle.z * lifeTime, particle.y + particle.w * lifeTime);
	float sum = (lifeTime * (lifeTime + 1)) / 2.0f;
	position += force * sum;
	gl_Position = vec4(position.x, position.y, 0.0f, 1.0f);
	gl_PointSize = 0.5f;
}

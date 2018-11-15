#version 420
layout(location = 0) in lowp vec4 particle;
layout(location = 1) in lowp vec2 startAndLife;
layout(location = 2) in lowp vec3 force;
layout(location = 3) in lowp float size;

uniform float time;

out float life;

void main()
{
	float lifeTime = time - startAndLife.x;
	float frictionSum = (1 - pow(force.z, lifeTime))/(1-force.z);
	vec2 position = vec2(particle.x + particle.z * frictionSum, particle.y + particle.w * frictionSum);
	float sum = (lifeTime * (lifeTime + 1)) / 2.0f;
	position += force.xy * sum;
	gl_Position = vec4(position.x, position.y, 0.0f, 1.0f);
	gl_PointSize = size;
	
	life = 1 - (lifeTime / startAndLife.y);
}

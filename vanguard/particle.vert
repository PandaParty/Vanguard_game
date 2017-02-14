#version 420
layout(location = 0) in vec4 particle;
layout(location = 1) in float startTime;

uniform float time;

void main()
{
	gl_Position = vec4(particle.x + particle.z * (time - startTime), particle.y + particle.w * (time - startTime), 0.0f, 1.0f);
	gl_PointSize = 4.0f;
}

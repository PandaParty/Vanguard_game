#version 420
layout(location = 0) in vec2 particle;

void main()
{
	gl_Position = vec4(particle.x, particle.y, 0.0f, 1.0f);
	gl_PointSize = 2.0f;
}

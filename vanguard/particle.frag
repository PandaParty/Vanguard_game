#version 420

precision highp float;

in float life;

void main()
{
	gl_FragColor = vec4 (0.2f, 0.05f, 0.05f, life);
}

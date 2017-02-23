#version 420

precision highp float;

uniform vec3 color;

layout(binding = 0) uniform sampler2D texture;

in float life;

void main()
{
	vec4 text = texture2D(texture, gl_PointCoord);
	gl_FragColor = vec4 (color + text.xyz * 0.2f, life * text.w);
}

#version 420

precision highp float;

in vec2 coord;

layout(binding = 0) uniform sampler2D sprite;

void main()
{
	gl_FragColor = texture2D(sprite, coord.xy);
	//gl_FragColor = vec4(1.0f);
}

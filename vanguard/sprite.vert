#version 420
layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texCoord;

uniform mat4 projection;
uniform mat4 model;

out vec2 coord;

void main()
{
	gl_Position = projection * model * vec4(position.xy, 0.0f, 1.0f);
	coord = texCoord;
}

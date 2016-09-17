// grass vertex shader

#version 410 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;

out vec2 TexCoords;

uniform float scaleX;
uniform float scaleY;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
	float th = scaleX;
	vec3 pos = vec3(position.x*scaleY,position.y*scaleY,position.z);
    gl_Position = projection * view * model * vec4(pos, 1.0f);
    TexCoords = texCoords;
}
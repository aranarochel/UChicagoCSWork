// wireframe and flat shading vertex shader 
#version 410 core

layout(location = 0) in vec4 position; 

uniform mat4 modelViewMat; 
uniform mat4 projection;
uniform float tau;
uniform float vertexError;

void main (void)
{
	// allow for morphing between LODs
	float morphDelta = position.w;
	float tmorph = morphDelta * clamp((2*vertexError/tau) - 1, 0, 1);

	vec3 pos = vec3(position.x,position.y + tmorph, position.z);

    gl_Position =  projection * modelViewMat * vec4(pos,1.0); 
}
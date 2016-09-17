//vertex shader for terrain with lighting, also allows for morphing and fog
#version 410 core

layout(location = 0) in vec4 position; 

uniform mat4 modelViewMat; 
uniform mat4 modelMat; 
uniform mat4 projection;
uniform float tileWidth;
uniform float tau;
uniform float vertexError;

out vec3 wPos;
out vec3 vPos;

void main (void)
{
	float morphDelta = position.w;
	float tmorph = morphDelta * clamp((2*vertexError/tau) - 1, 0, 1);
	vec3 pos = vec3(position.x,position.y + tmorph, position.z);


    gl_Position =  projection * modelViewMat * vec4(pos,1.0); 
    wPos = pos;
    // get view position for calculating the distance to the fog bank
    vPos = (modelMat * vec4(pos,1.0)).xyz; 
}
//Vertex shader for dynamic horizons, allowing for slection of 1 or 2 axis horizon drop off
#version 410 core

layout(location = 0) in vec4 position; 

uniform mat4 modelViewMat; 
uniform mat4 modelMat; 
uniform mat4 projection;
uniform float tileWidth;
uniform float tau;
uniform float vertexError;
uniform vec3 camPos;

uniform float Curvature;
uniform bool isSpherical;

out vec3 wPos;
out vec3 vPos;

void main (void)
{
	mat4 ta = modelMat;
	float morphDelta = position.w;
	float tmorph = morphDelta * clamp((2*vertexError/tau) - 1, 0, 1);
	vec3 pos = vec3(position.x,position.y + tmorph, position.z);
    vPos = (modelMat * vec4(pos,1.0)).xyz; 

	vec4 vv = projection * modelViewMat * vec4(pos,1.0);
	if (isSpherical)
	{
		vv -= vec4(camPos*0.0 - vv.xyz,0.0);
		vv = vec4(0.0,((vv.z * vv.z) + (vv.x * vv.x)) * Curvature, 0.0, 0.0);		
	}
	else
	{
		vv -= vec4(camPos - vv.xyz,0.0);
		vv = vec4(0.0,(vv.z * vv.z) * Curvature, 0.0, 0.0);
	}
	gl_Position = projection * modelViewMat * vec4(pos,1.0) - vv; 

    wPos = pos;
    // get view position for calculating the distance to the fog bank
}
/*! \file shader.vert
 *
 * \brief The vertex shader for a cube
 *
 * \author Lamont Samuels
 *
 */
#version 410 core

uniform mat4 modelView; 
uniform mat4 projection;
uniform mat4 modelViewLight;
uniform mat4 projectionLight;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tCoord;

out vec2 f_tCoord;
out vec3 f_normal; 
out vec4 shadowCoord;


void main (void)
{
    gl_Position =  projection * modelView * vec4(position,1.0); 
 	f_tCoord = tCoord;
 	f_normal = normal;

 	shadowCoord = projectionLight * modelViewLight * vec4(position,1.0);
}
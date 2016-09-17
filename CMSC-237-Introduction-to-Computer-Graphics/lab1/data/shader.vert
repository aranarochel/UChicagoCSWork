/*! \file shader.vert
 *
 * \brief The vertex shader for a cube
 *
 * \author Lamont Samuels
 *
 */
#version 410 core

layout(location = 0) in vec3 position; 
layout(location = 1) in vec4 color;

out vec4 vColor;

uniform mat4 camera; 
uniform mat4 projection;


void main (void)
{
    gl_Position =  projection * camera * vec4(position,1.0); 
    vColor = color; 
}

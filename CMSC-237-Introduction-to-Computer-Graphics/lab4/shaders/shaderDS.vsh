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

layout (location = 0) in vec3 position;

void main (void)
{
    gl_Position =  projection * modelView * vec4(position,1.0); 
}
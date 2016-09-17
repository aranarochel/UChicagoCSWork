/*! \file no-light.vsh
 *
 * \brief Vertex shader for drawing the wireframe
 *
 * \author John Reppy
 */

/* CMSC23700 Project 4 sample code (Autumn 2015)
 *
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#version 410

uniform mat4 mvpMat;			// model-view-projectiom transform

layout (location = 0) in vec3 coord;	// vertex position

void main ()
{
    gl_Position = mvpMat * vec4(coord, 1);
}

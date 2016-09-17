/*! \file quad.vsh
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

vec2 texs[6] = vec2[] ( vec2(0,0),
					    vec2(1,1),
					    vec2(0,1),
					    vec2(0,0),
					    vec2(1,0),
					    vec2(1,1));

vec3 verts[6] = vec3[] (vec3(-1,-1,0),
						vec3(1,1,0),
						vec3(-1,1,0),
						vec3(-1,-1,0),
						vec3(1,-1,0),
						vec3(1,1,0));

out vec2 texCoord;


void main ()
{
	texCoord = texs[gl_VertexID];
    gl_Position = vec4(verts[gl_VertexID],1);
}

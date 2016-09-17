/*! \file quad.fsh
 *
 * \brief Fragment shader for drawing the wireframe
 *
 * \author John Reppy
 */

/* CMSC23700 Project 4 sample code (Autumn 2015)
 *
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#version 410

in vec2 texCoord;
uniform sampler2D color;

out vec4 fragColor;

void main ()
{
    fragColor = texture(color,texCoord);
}

/*! \file texture.fsh
 *
 * \brief The fragment shader for the Gbuffer
 *
 * \author John Reppy
 */

/* CMSC23700 Project 4 sample code (Autumn 2015)
 *
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#version 410


uniform vec3  diffuseC;   // diffuse color when there is no diffuse map
uniform sampler2D diffuseMap;   // the object's diffuse-color-map texture
uniform bool  hasDiffuseMap;    // true when there is a diffuse map.

in VS_OUT {
    vec3	norm;		// interpolated vertex normal in world coordinates
    vec3  coord;    // interpolated vertex position in world coordinates
    smooth vec2 texCoord; // vertex tex coordinate
} fsIn;

layout (location = 0) out vec3 pOut;
layout (location = 1) out vec4 cOut;
layout (location = 2) out vec3 nOut;

void main ()
{

  // surface color
  vec3 surfaceC;
  if (hasDiffuseMap) {
surfaceC = texture(diffuseMap, fsIn.texCoord).rgb;
  } else {
surfaceC = diffuseC;
  }

  pOut = fsIn.coord;
  cOut = vec4(surfaceC,1);
  nOut = normalize(fsIn.norm);

}

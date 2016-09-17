/*! \file texture.fsh
 *
 * \brief The fragment shader for rendering in TEXTURING mode
 *
 * \author John Reppy
 */

/* CMSC23700 Project 4 sample code (Autumn 2015)
 *
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#version 410

uniform vec3	ambIntensity;		// ambient light intensity
uniform vec3	lightIntensity;		// the light's intensity
uniform vec3	lightDir;		// the light's direction in world space
uniform vec3	diffuseC;		// diffuse color when there is no diffuse map
uniform sampler2D diffuseMap;		// the object's diffuse-color-map texture
uniform bool	hasDiffuseMap;		// true when there is a diffuse map.

in VS_OUT {
    vec3	norm;		// interpolated vertex normal in world coordinates
    smooth vec2	texCoord;	// vertex tex coordinate
} fsIn;

layout (location = 0) out vec4 fragColor;

void main ()
{
  // renormalize the surface normal
    vec3 norm = normalize(fsIn.norm);

  // direct-lighting contribution
    vec3 intensity = max(dot(lightDir, norm), 0.0) * lightIntensity;

  // surface color
    vec3 surfaceC;
    if (hasDiffuseMap) {
	surfaceC = texture(diffuseMap, fsIn.texCoord).rgb;
    } else {
	surfaceC = diffuseC;
    }

  // the fragment color is the ambient color plus the lighting
    vec3 c = clamp ((ambIntensity + intensity) * surfaceC, 0, 1);

    fragColor = vec4(c, 1.0);

}

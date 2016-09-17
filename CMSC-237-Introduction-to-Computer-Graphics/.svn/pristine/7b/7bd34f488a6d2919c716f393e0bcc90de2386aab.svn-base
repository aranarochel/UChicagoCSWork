/*! \file shader.vert
 *
 * \brief The fragment shader for a cube 
 *
 * \author Lamont Samuels 
 *
 */
#version 410 core 

uniform sampler2D sampler;

/** Uniforms used for lighting **/
uniform vec3 direction;
uniform vec3 ambient;
uniform vec3 intensity;

in vec2 f_tCoord;
in vec3 f_normal;

out vec4 fragColor;

void main(void)
{
	vec3 norm = normalize(f_normal);



	fragColor = texture(sampler,f_tCoord);
}
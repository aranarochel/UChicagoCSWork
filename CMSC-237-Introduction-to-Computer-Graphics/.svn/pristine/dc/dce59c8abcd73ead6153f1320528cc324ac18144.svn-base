/*! \file shader.vert
 *
 * \brief The fragment shader for a cube 
 *
 * \author Lamont Samuels 
 *
 */
#version 410 core 

/** Uniforms used for lighting **/
uniform vec3 direction;
uniform vec3 ambient;
uniform vec3 intensity;

in vec2 f_tCoord;
in vec3 f_normal;

uniform vec4 Vcolor;
out vec4 color;

void main(void)
{
	vec3 norm = normalize(f_normal);

	color = vec4(ambient + (max(0,dot(-direction,norm)) * intensity),1) * Vcolor;
}
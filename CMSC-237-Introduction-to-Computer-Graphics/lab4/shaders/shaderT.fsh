/*! \file shader.vert
 *
 * \brief The fragment shader for a cube 
 *
 * \author Lamont Samuels 
 *
 */
#version 410 core 

uniform sampler2DShadow samplerShadow;
uniform sampler2D sampler;

uniform vec3 direction;
uniform vec3 ambient;
uniform vec3 intensity;

in vec2 f_tCoord;
in vec3 f_normal;
in vec4 shadowCoord;


out vec4 color;

void main(void)
{
	vec3 norm = normalize(f_normal);

	vec3 sCoord = vec3(shadowCoord)/shadowCoord.w;
	sCoord = (0.5*sCoord) + 0.5;
	float sfactor = (0.5 * texture(samplerShadow,vec3(sCoord.x,sCoord.y,sCoord.z-0.002))) + 0.5;

	color = (sfactor * vec4(ambient + (max(0,dot(-direction,norm)) * intensity),1)) * texture(sampler,f_tCoord);
}
//grass fragment shader
#version 410 core

uniform sampler2D grassTex;
uniform vec3 ambientIntensity;
uniform vec3 sunIntensity;
in vec2 TexCoords;

out vec4 color;

void main()
{
	vec4 texCol = texture(grassTex, TexCoords);

	if(texCol.a < 0.1)
		discard;

	color = texCol * vec4(ambientIntensity+(ambientIntensity * sunIntensity),1);
}
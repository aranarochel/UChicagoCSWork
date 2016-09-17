#version 410

in vec2 v_texs;
uniform sampler2D tex;
layout (location = 0) out vec4 fragColor;

void main()
{
	//"blast" the texture to the screen
	fragColor = texture(tex, v_texs);
}
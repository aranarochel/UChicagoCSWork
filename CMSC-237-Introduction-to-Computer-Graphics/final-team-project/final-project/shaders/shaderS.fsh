//Skybox fragment shader samples thr skybox cube
#version 410 core

in vec3 TexCoords;
out vec4 color;

uniform samplerCube skybox;

void main()
{    
    color = texture(skybox, TexCoords);
}
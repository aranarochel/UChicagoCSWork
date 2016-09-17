//Fragment shader for scene with lighting, fog, and morphing
#version 410 core 

uniform vec3 direction;
uniform vec3 ambient;
uniform vec3 intensity;

uniform vec4 color;
uniform sampler2D colorTexture;
uniform sampler2D normalTexture;
uniform float tileWidth;
uniform vec3 NWCorner;

uniform vec4 fogFactors;
uniform vec3 camPos;
uniform bool hasFog;


in vec3 wPos; //world position;
in vec3 vPos; //view position;

out vec4 fragColor;

void main(void)
{
	//calculate the texture coordinates depending on where this frag is, where the tile is, and the size of the tile
	vec2 texCoord = (wPos.xz - NWCorner.xz) / tileWidth;
	// use texCoords to pull out the interesting bits
	vec4 texCol = texture(colorTexture, texCoord);
	vec3 texNorm = vec3(texture(normalTexture, texCoord));
	vec3 texNormTransform = vec3(texNorm.x,texNorm.y,texNorm.z);
	// shift the normals to be the correct orientation
	texNormTransform = 2*texNormTransform - 1;
	float fogDist = 0.0;
	// find a default fragColor
	fragColor = vec4(ambient + (max(0,dot(-direction,texNormTransform))) * intensity, 1) * texCol;
	// if there's fog, we'll add that
	if (hasFog)
	{
		//calculate the distance from the camera
		fogDist = length(camPos - vPos);
		// calculate the amount of fog
		float ffactor = 1 / exp(pow(fogFactors.w * fogDist,2));
		// scale fragColor by fog factor
		fragColor = vec4((1.0 - ffactor)*(fogFactors.rgb),1.0) + (ffactor)*(fragColor);
	}
}
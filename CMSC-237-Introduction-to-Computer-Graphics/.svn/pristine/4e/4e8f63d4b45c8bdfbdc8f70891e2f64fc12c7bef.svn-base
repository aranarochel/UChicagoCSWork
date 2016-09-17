/*! \file deferred.fsh
 *
 * \brief The fragment shader for rendering in Deffered mode
 *
 * \author John Reppy
 */

/* CMSC23700 Project 4 sample code (Autumn 2015)
 *
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#version 410

uniform float  lightType;

uniform sampler2D _samplerPos;
uniform sampler2D _samplerDiff;
uniform sampler2D _samplerNorm;

uniform vec3	ambIntensity;		// ambient light intensity
uniform vec3	lightIntensity;		// the light's intensity
uniform vec3	lightDir;		// the light's direction in world space

uniform vec3  spotPos;
uniform vec3  spotDir;
uniform vec3  spotAtt;
uniform float spotCutt;
uniform float spotExp;
uniform vec3  camPos;
uniform vec2  windowSize;
uniform vec3  specularC;
uniform float shininess;

layout (location = 3) out vec4 sOut;

void main ()
{

  vec3  a = lightIntensity;

  vec2 texCoords = gl_FragCoord.xy / windowSize;

  // G-Buffer uniforms
  vec3 pos = texture(_samplerPos, texCoords).rgb;
  vec3 diff = texture(_samplerDiff, texCoords).rgb;
  vec3 norm_temp = texture(_samplerNorm, texCoords).rgb;
  vec3 norm = normalize(norm_temp);

  float specular = 0.0;
  vec3 col;
  float attenuation;

  if(lightType == 1.0) {  // directional light
      
    // Blinn-Phong Specular Calculation
    vec3  v = lightDir;
    float lambertian = max(0.0, dot(norm, v));

    //specular
    if(lambertian > 0.0) {

      vec3 viewDir = normalize(camPos - pos.xyz);
      vec3 halfDir = normalize(v + viewDir);
      float specAngle = max(dot(halfDir, norm), 0.0);
      specular = pow(specAngle, shininess);
    }
    col = ambIntensity * diff + specular * specularC;

  } else if(lightType == 2.0){    // spotlights

    // spotlight calculations
    float attenC = spotAtt.x;
    float attenB = spotAtt.y;
    float attenA = spotAtt.z;

    vec3  v = normalize(spotPos - pos.xyz);
    float vDis = length(v);
    float cosAngle = dot(v,spotDir) / vDis;
    attenuation = (1.0 / (attenA*pow(vDis,2) + attenB*vDis + attenC)) * pow(cosAngle,spotExp);
    
    // test for spotlight intensity
    if (cosAngle > cos(radians(spotCutt))) {
      attenuation = 0.0;
    }

    // Blinn-Phong Specular Calculation
    float lambertian = max(0.0, dot(norm, v));

    //specular
    if(lambertian > 0.0) {

      vec3 viewDir = normalize(camPos - pos.xyz);
      vec3 halfDir = normalize(v + viewDir);
      float specAngle = max(dot(halfDir, norm), 0.0);
      specular = pow(specAngle, shininess);
    }

    col = clamp(max(dot(norm,v),0) * diff,0,1) * attenuation + specular * specularC;
  }

  sOut = vec4(col, 1.0);

}

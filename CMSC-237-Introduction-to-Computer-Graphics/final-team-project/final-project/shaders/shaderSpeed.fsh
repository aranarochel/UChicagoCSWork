#version 410 core 
// modified from Warp Speed 2 by Dave_Hoskins
// https://www.shadertoy.com/view/4tjSDt

uniform vec2 resolution;
uniform float globalTime;
uniform bool isTurning;

in vec2 v_texs;
out vec4 fragColor;

void main()
{
	float s = 0.0, v = 0.0;
	vec2 uv = (gl_FragCoord.xy / (resolution.xy)) * 2.0 - 1.;
	vec2 xy = (gl_FragCoord.xy / (resolution.xy)) * 2.0 - 1.;
    float time = (globalTime-1.0)*60.0;
	vec3 col = vec3(0);
    vec3 init = vec3(sin(time * .0032)*.3, .35 - cos(time * .005)*.3, time * 0.002);
	for (int r = 0; r < 100; r++) 
	{
		vec3 p = init + s * vec3(uv, 0.05);
		p.z = fract(p.z);
        // Thanks to Kali's little chaotic loop...
		for (int i=0; i < 10; i++)	p = abs(p * 2.04) / dot(p, p) - .9;
		v += pow(dot(p, p), .7) * .06;
		col +=  vec3(v * 0.2+.4, 12.-s*2., .1 + v * 1.) * v * 0.00003;
		s += .025;
	}
	// if we're turning, use blue. if we're moving, use orange
	if (!isTurning)
		col = vec3(col.z,col.y,col.x);
	fragColor = vec4(clamp(col, 0.0, 1.0), 0.5);
	// make scene more visible by reducing opacity near the center of the screen
	vec2 center = vec2(0,0);
	fragColor.w = pow(length(xy - center),2.);
}
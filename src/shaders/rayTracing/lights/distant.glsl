//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// distant.glsl
// Date: 2021-02-13
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef LIGHTS_DISTANT_GLSL
#define LIGHTS_DISTANT_GLSL
#include "base.glsl"

// Current assumption: The scene sphere diameter is less than 10000 meters

float DistantLight_falloff(Light light, vec3 w, float cosTotalWidth, float cosFalloffStart);

vec3 DistantLight_sampleLi(
		Light light, Interaction ref, vec2 u, 
		out vec3 wi, out float pdf, out VisibilityTester vis, 
		vec3 L, vec3 wLight)
{
	wi = wLight;
	pdf = 1.f;
	vec3 pOutside = ref.point + wLight*10000;
	vis.p0 = ref;
	vis.p1.point = pOutside;

	return L;
}

vec3 DistantLight_power(vec3 L)
{
	return L * 3.1415926535f * 5000*5000;
}

uint DistantLight_flags()
{
	return LIGHT_FLAG_DELTA_POSITION;
}

#endif// LIGHTS_DISTANT_GLSL
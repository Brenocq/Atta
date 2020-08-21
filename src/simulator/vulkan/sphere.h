//--------------------------------------------------
// Robot Simulator
// sphere.h
// Date: 22/07/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef SPHERE_H
#define SPHERE_H

#include "procedural.h"
#include "glm.h"

class Sphere final : public Procedural
{
public:
	Sphere(const glm::vec3& center, const float radius):
		center(center), radius(radius)
	{
	}

	const glm::vec3 center;
	const float radius;

	std::pair<glm::vec3, glm::vec3> boundingBox() const override
	{
		return std::make_pair(center - radius, center + radius);
	}

};

#endif// SPHERE_H
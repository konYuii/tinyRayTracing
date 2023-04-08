#pragma once
#ifndef RAY_H
#define RAY_H
#include "vec3.h"

class ray {
public:
	vec3 orig;
	vec3 dir;

	ray() {}
	ray(vec3 o, vec3 d)
		:orig(o), dir(d) {}

	vec3 origin() const { return orig; }
	vec3 direction() const { return dir; }

	vec3 at(double t) const {
		return orig + t * dir;
	}
};


#endif // !RAY_H


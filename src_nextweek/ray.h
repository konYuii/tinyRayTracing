#pragma once
#ifndef RAY_H
#define RAY_H
#include "vec3.h"

class ray {
public:
	vec3 orig;
	vec3 dir;
	double tm;

	ray() {}
	ray(vec3 o, vec3 d)
		:orig(o), dir(d), tm(0) {}
	ray(vec3 o, vec3 d, double t)
		:orig(o), dir(d), tm(t) {}

	vec3 origin() const { return orig; }
	vec3 direction() const { return dir; }
	double time() const { return tm; }

	vec3 at(double t) const {
		return orig + t * dir;
	}
};


#endif // !RAY_H


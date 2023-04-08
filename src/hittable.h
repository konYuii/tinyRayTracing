#pragma once
#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
class material;

struct hit_record {
	point3 p;
	vec3 normal;
	double t;
	bool front_face;
	shared_ptr<material> mat;

	inline void set_face_normal(const ray& r, const vec3& out_normal)
	{
		front_face = dot(r.direction(), out_normal) < 0;
		normal = front_face ? out_normal : -1 * out_normal;
	}
};

class hittable {
public:
	virtual bool hit(const ray&, double, double, hit_record&)const = 0;
};


#endif // !HITTABLE_H


#pragma once
#ifndef AABB_H
#define AABB_H

#include "utility.h"
#include "vec3.h"
#include "ray.h"

class aabb {
public:
	point3 minimum;
	point3 maximum;

	aabb() {}
	aabb(point3 mi, point3 ma)
		:minimum(mi), maximum(ma) {}

	point3 min() const { return minimum; }
	point3 max() const { return maximum; }

	inline bool hit(const ray& r, double t_min, double t_max) const {
		for (int i = 0; i < 3; i++)
		{
			double inv = 1.0 / r.direction()[i];
			double entry = (min()[i] - r.origin()[i]) * inv;
			double exit = (max()[i] - r.origin()[i]) * inv;

			if (inv < 0)
				std::swap(entry, exit);
			t_min = entry > t_min ? entry : t_min;
			t_max = exit < t_max ? exit : t_max;
			if (t_max <= t_min)
				return false;
		}
		return true;
	}


};

aabb surrounding_box(aabb box0, aabb box1) {
	point3 small(fmin(box0.min().x(), box1.min().x()),
		fmin(box0.min().y(), box1.min().y()),
		fmin(box0.min().z(), box1.min().z()));

	point3 big(fmax(box0.max().x(), box1.max().x()),
		fmax(box0.max().y(), box1.max().y()),
		fmax(box0.max().z(), box1.max().z()));

	return aabb(small, big);
}

#endif // !AABB_H

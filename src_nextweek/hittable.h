#pragma once
#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "aabb.h"
class material;

struct hit_record {
	point3 p;
	vec3 normal;
	double t;
	bool front_face;
	shared_ptr<material> mat;
	double u;
	double v;

	inline void set_face_normal(const ray& r, const vec3& out_normal)
	{
		front_face = dot(r.direction(), out_normal) < 0;
		normal = front_face ? out_normal : -1 * out_normal;
	}
};

class hittable {
public:
	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec)const = 0;
	virtual bool bounding_box(double time0, double time1, aabb& output_box) const = 0;
};


class translate : public hittable {
private:
	shared_ptr<hittable> ptr;
	vec3 offset;
public:
	translate() {}
	translate(shared_ptr<hittable> p, const vec3& move)
		:ptr(p), offset(move) {}

	virtual bool hit(const ray&, double t_min, double t_max, hit_record&)const override;
	virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;
};

class rotate_y : public hittable {
private:
	shared_ptr<hittable> ptr;
	double sin_theta;
	double cos_theta;
	bool hasbox;
	aabb bbox;

public:
	rotate_y() {}
	rotate_y(shared_ptr<hittable> obj, double angle);

	virtual bool hit(const ray&, double t_min, double t_max, hit_record&)const override;

	virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
		output_box = bbox;
		return hasbox;
	}

};


bool translate::hit(const ray& r, double t_min, double t_max, hit_record& rec)const
{
	ray move_r(r.origin() - offset, r.direction(), r.time());
	if (!ptr.get()->hit(move_r, t_min, t_max, rec))
		return false;

	rec.p += offset;
	rec.set_face_normal(move_r, rec.normal);

	return true;
}
bool translate::bounding_box(double time0, double time1, aabb& output_box) const
{
	if (!ptr.get()->bounding_box(time0, time1, output_box))
		return false;

	output_box = aabb(output_box.min() + offset,
		output_box.max() + offset);
	return true;
}

rotate_y::rotate_y(shared_ptr<hittable> obj, double angle)
{
	ptr = obj;
	double radius = degrees_to_radians(angle);
	cos_theta = cos(radius);
	sin_theta = sin(radius);
	hasbox = obj.get()->bounding_box(0, 1, bbox);

	point3 minm(infinity, infinity, infinity);
	point3 maxm(-infinity, -infinity, -infinity);

	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
			for (int k = 0; k < 2; k++)
			{
				double x = i * bbox.min().x() + (1.0 - i) * bbox.max().x();
				double y = j * bbox.min().y() + (1.0 - j) * bbox.max().y();
				double z = k * bbox.min().z() + (1.0 - k) * bbox.max().z();

				double new_x = cos_theta * x + sin_theta * z;
				double new_z = -sin_theta * x + cos_theta * z;

				vec3 tester(new_x, y, z);
				for (int c = 0; c < 3; c++) {
					minm[c] = fmin(minm[c], tester[c]);
					maxm[c] = fmax(maxm[c], tester[c]);
				}
			}

	bbox = aabb(minm, maxm);

}
bool rotate_y::hit(const ray& r, double t_min, double t_max, hit_record& rec)const {
	auto origin = r.origin();
	auto direction = r.direction();

	origin[0] = cos_theta * r.origin()[0] - sin_theta * r.origin()[2];
	origin[2] = sin_theta * r.origin()[0] + cos_theta * r.origin()[2];

	direction[0] = cos_theta * r.direction()[0] - sin_theta * r.direction()[2];
	direction[2] = sin_theta * r.direction()[0] + cos_theta * r.direction()[2];

	ray new_r(origin, direction, r.time());

	if (!ptr.get()->hit(new_r, t_min, t_max, rec))
	{
		return false;
	}

	auto p = rec.p;
	auto normal = rec.normal;

	p[0] = cos_theta * rec.p[0] + sin_theta * rec.p[2];
	p[2] = -sin_theta * rec.p[0] + cos_theta * rec.p[2];

	normal[0] = cos_theta * rec.normal[0] + sin_theta * rec.normal[2];
	normal[2] = -sin_theta * rec.normal[0] + cos_theta * rec.normal[2];

	rec.p = p;
	rec.set_face_normal(new_r, normal);

	return true;

}

#endif // !HITTABLE_H


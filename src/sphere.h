#pragma once
#include "hittable.h"

class sphere : public hittable {
public:
	point3 center;
	double radius;
	shared_ptr<material> mat;

	sphere() {}
	sphere(point3 c, double r, shared_ptr<material> m)
		:center(c), radius(r), mat(m) {}

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const
{
	vec3 v = r.orig - center;
	double a = r.direction().length_squared();
	double h_b = dot(r.dir, v);
	double c = dot(v, v) - radius * radius;

	double delta = h_b * h_b - a * c;
	if (delta < 0)
		return false;

	double t = (-h_b - sqrt(delta)) / a;
	if (t<t_min || t>t_max)
	{
		t = (-h_b + sqrt(delta)) / a;
		if (t<t_min || t>t_max)
			return false;
	}
	//std::cerr << t << '\n';
	rec.p = r.at(t);
	rec.t = t;
	vec3 out_normal = (rec.p - center) / radius;
	rec.set_face_normal(r, out_normal);
	rec.mat = mat;

	return true;
}

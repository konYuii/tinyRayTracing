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
	virtual bool bounding_box(double time0, double time1, aabb& ouput_box) const override;

private:
	static void get_sphere_uv(const point3& p, double& u, double& v)
	{
		double theta = acos(-p.y());
		double phi = atan2(-p.z(), p.x()) + pi;

		u = phi / (2 * pi);
		v = theta / pi;
	}
};

class moving_sphere : public hittable {
public:
	point3 center0, center1;
	double radius;
	shared_ptr<material> mat;
	double time0;
	double time1;

	moving_sphere(point3 c0, point3 c1, double t0, double t1, double r, shared_ptr<material> m)
		:center0(c0), center1(c1), radius(r), mat(m), time0(t0), time1(t1) {}

	vec3 center(double time) const;

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
	virtual bool bounding_box(double time0, double time1, aabb& ouput_box) const override;
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
	get_sphere_uv(out_normal, rec.u, rec.v);
	rec.mat = mat;

	return true;
}

vec3 moving_sphere::center(double time) const
{

	return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
}

bool moving_sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const
{
	vec3 t_center = center(r.time());
	vec3 v = r.orig - t_center;
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
	vec3 out_normal = (rec.p - t_center) / radius;
	rec.set_face_normal(r, out_normal);
	rec.mat = mat;

	return true;
}

bool sphere::bounding_box(double time0, double time1, aabb& output_box) const
{
	output_box = aabb(
		center - vec3(radius, radius, radius),
		center + vec3(radius, radius, radius)
	);
	return true;

}
bool moving_sphere::bounding_box(double time0, double time1, aabb& output_box) const
{
	aabb a0 = aabb(
		center(time0) - vec3(radius, radius, radius),
		center(time0) + vec3(radius, radius, radius)
	);
	aabb a1 = aabb(
		center(time1) - vec3(radius, radius, radius),
		center(time1) + vec3(radius, radius, radius)
	);

	output_box = surrounding_box(a0, a1);
	return true;
}
#pragma once
#ifndef AARECT_H
#define AARECT_H

#include "hittable.h"

class xy_rect : public hittable {
private:
	double x0, y0, x1, y1, k;
	shared_ptr<material> mat;
public:
	xy_rect() {}
	xy_rect(double _x0, double _x1, double _y0, double _y1, double _k,
		shared_ptr<material> m)
		: x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mat(m) {};

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec)const override;
	virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
		output_box = aabb(point3(x0, y0, k - 0.0001), point3(x1, y1, k + 0.0001));
		return true;
	}
};

class yz_rect : public hittable {
private:
	double y0, z0, y1, z1, k;
	shared_ptr<material> mat;
public:
	yz_rect() {}
	yz_rect(double _y0, double _y1, double _z0, double _z1, double _k,
		shared_ptr<material> m)
		: y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), mat(m) {};

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec)const override;
	virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
		output_box = aabb(point3(k - 0.0001, y0, z0), point3(k + 0.0001, y1, z1));
		return true;
	}
};
class xz_rect : public hittable {
private:
	double x0, z0, x1, z1, k;
	shared_ptr<material> mat;
public:
	xz_rect() {}
	xz_rect(double _x0, double _x1, double _z0, double _z1, double _k,
		shared_ptr<material> m)
		: x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), mat(m) {};

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec)const override;
	virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
		output_box = aabb(point3(x0, k - 0.0001, z0), point3(x1, k + 0.0001, z1));
		return true;
	}
};




bool xy_rect::hit(const ray& r, double t_min, double t_max, hit_record& rec)const {
	double t = (k - r.origin().z()) / r.direction().z();

	if (t<t_min || t>t_max)
		return false;

	double x = r.origin().x() + t * r.direction().x();
	double y = r.origin().y() + t * r.direction().y();

	if (x<x0 || x>x1 || y<y0 || y>y1)
		return false;
	//std::cerr << t << '\n';

	rec.mat = mat;
	rec.t = t;
	rec.p = vec3(x, y, k);
	vec3 normal = vec3(0, 0, 1);
	rec.u = (x - x0) / (x1 - x0);
	rec.v = (y - y0) / (y1 - y0);
	rec.set_face_normal(r, normal);


	return true;
}

bool yz_rect::hit(const ray& r, double t_min, double t_max, hit_record& rec)const {
	double t = (k - r.origin().x()) / r.direction().x();

	if (t<t_min || t>t_max)
		return false;

	double y = r.origin().y() + t * r.direction().y();
	double z = r.origin().z() + t * r.direction().z();

	if (z<z0 || z>z1 || y<y0 || y>y1)
		return false;

	rec.mat = mat;
	rec.t = t;
	rec.p = r.at(t);
	vec3 normal = vec3(1, 0, 0);
	rec.v = (z - z0) / (z1 - z0);
	rec.u = (y - y0) / (y1 - y0);
	rec.set_face_normal(r, normal);

	return true;
}

bool xz_rect::hit(const ray& r, double t_min, double t_max, hit_record& rec)const {
	double t = (k - r.origin().y()) / r.direction().y();

	if (t<t_min || t>t_max)
		return false;
	double x = r.origin().x() + t * r.direction().x();
	double z = r.origin().z() + t * r.direction().z();

	if (x<x0 || x>x1 || z<z0 || z>z1)
		return false;

	rec.mat = mat;
	rec.t = t;
	rec.p = r.at(t);
	vec3 normal = vec3(0, 1, 0);
	rec.u = (x - x0) / (x1 - x0);
	rec.v = (z - z0) / (z1 - z0);
	rec.set_face_normal(r, normal);

	return true;
}

#endif // !AARECT_H

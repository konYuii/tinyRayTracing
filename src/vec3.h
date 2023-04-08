#pragma once
#ifndef VEC3_H
#define VEC3_H

#include<iostream>
#include<cmath>
#include "utility.h"

class vec3 {
public:
	double e[3];

	vec3() :e{ 0,0,0 } {}
	vec3(double e1, double e2, double e3)
		:e{ e1,e2,e3 } {}

	const double x() { return e[0]; }
	const double y() { return e[1]; }
	const double z() { return e[2]; }

	vec3 operator-() { return { -e[0],-e[1],-e[2] }; }
	double& operator[](int i) { return e[i]; }

	vec3& operator+=(const vec3 v)
	{
		e[0] += v.e[0];
		e[1] += v.e[1];
		e[2] += v.e[2];
		return *this;
	}
	vec3& operator*=(const vec3 v)
	{
		e[0] *= v.e[0];
		e[1] *= v.e[1];
		e[2] *= v.e[2];
		return *this;
	}
	vec3& operator/=(const vec3 v)
	{
		e[0] /= v.e[0];
		e[1] /= v.e[1];
		e[2] /= v.e[2];
		return *this;
	}

	double length_squared()
	{
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}
	double length()
	{
		return sqrt(length_squared());
	}

	inline static vec3 random() {
		return vec3(random_double(), random_double(), random_double());
	}
	inline static vec3 random(double min, double max) {
		return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
	}
};

using point3 = vec3;
using color = vec3;

inline std::ostream& operator<<(std::ostream& out, const vec3 v)
{
	return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2] << '\n';
}
inline vec3 operator+(const vec3 u, const vec3 v)
{
	return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}
inline vec3 operator-(const vec3 u, const vec3 v)
{
	return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3 u, const vec3 v)
{
	return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}
inline vec3 operator*(const vec3 v, double t)
{
	return vec3(v.e[0] * t, v.e[1] * t, v.e[2] * t);
}
inline vec3 operator*(double t, const vec3 v)
{
	return vec3(v.e[0] * t, v.e[1] * t, v.e[2] * t);
}
inline vec3 operator/(const vec3 v, double t)
{
	return vec3(v.e[0] / t, v.e[1] / t, v.e[2] / t);
}

inline double dot(const vec3 u, const vec3 v)
{
	return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}
inline vec3 cross(const vec3 u, const vec3 v)
{
	return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
		u.e[2] * v.e[0] - u.e[0] * v.e[2],
		u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}
inline vec3 unit_vector(vec3 v)
{
	return v / v.length();
}
vec3 random_in_unit_sphere() {

	double theta = random_double(0, 2 * pi);
	double cosphi = random_double(-1, 1);
	double sinphi = sqrt(1 - cosphi * cosphi);

	return vec3(sinphi * cos(theta), sinphi * sin(theta), cosphi);
}
vec3 random_in_unit_disk() {
	while (true) {
		auto p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
		if (p.length_squared() >= 1) continue;
		return p;
	}
}


inline vec3 reflect(const vec3& v, const vec3& n)
{
	return v - 2 * dot(v, n) * n;
}
inline vec3 refract(vec3& v, const vec3& n, double etai_over_etat)
{
	double c1 = dot(-v, n);
	double c2 = sqrt(1 - etai_over_etat * etai_over_etat * (1 - c1 * c1));

	return etai_over_etat * (v + c1 * n) - c2 * n;
}
#endif // VEC3_H


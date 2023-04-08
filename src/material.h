#pragma once
#include "hittable.h"

class material {
public:
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) = 0;
};

class lambertian : public material {
public:
	vec3 albedo;

	lambertian(const vec3& a)
		:albedo(a) {}

	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) override
	{
		scattered = ray(rec.p, rec.normal + random_in_unit_sphere());
		attenuation = albedo;
		return true;
	}
};

class metal : public material {
public:
	vec3 albedo;
	double fuzz;

	metal(const vec3& a, double fu)
		:albedo(a), fuzz(fu < 1 ? fu : 1) {}

	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) override
	{
		vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
		scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}
};

class dielectric : public material {
public:
	double ir;

	dielectric(double r)
		:ir(r) {}

	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered)
	{
		vec3 in = unit_vector(r_in.direction());
		double etai_over_etat = rec.front_face ? 1.0 / ir : ir;
		attenuation = color(1.0, 1.0, 1.0);

		double costheta = dot(-in, rec.normal);
		double sintheta = sqrt(1 - costheta * costheta);

		vec3 dir;
		if (etai_over_etat * sintheta > 1)
			dir = reflect(unit_vector(r_in.direction()), rec.normal);

		else
		{
			double posib = reflectance(costheta, etai_over_etat);
			if (random_double() < posib)
				dir = reflect(unit_vector(r_in.direction()), rec.normal);
			else
				dir = refract(in, rec.normal, etai_over_etat);
		}



		scattered = ray(rec.p, dir);
		return true;


	}
private:
	static double reflectance(double cosine, double ref)
	{
		auto r0 = (1.0 - ref) / (1.0 + ref);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow(1 - cosine, 5);
	}
};
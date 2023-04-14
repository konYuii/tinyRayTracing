#pragma once

#include "hittable.h"

#include<memory>
#include<vector>

using std::shared_ptr;
using std::make_shared;

class hittable_list : public hittable {
public:
	std::vector<shared_ptr<hittable>> objects;

	hittable_list() {}
	hittable_list(shared_ptr<hittable> object) { add(object); }

	void clear() { objects.clear(); }
	void add(shared_ptr<hittable> object) { objects.push_back(object); }

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

	virtual bool bounding_box(double time0, double time1, aabb& ouput_box) const override;
};

bool hittable_list::hit(const ray& r, double t_min, double t_max, hit_record& rec) const
{
	hit_record tmp_rec;
	double closest_to_far = t_max;
	bool hit_any = false;
	for (const auto& obj : objects)
	{
		if (obj.get()->hit(r, t_min, closest_to_far, tmp_rec))
		{
			hit_any = true;
			closest_to_far = tmp_rec.t;
			rec = tmp_rec;
		}
	}

	return hit_any;
}

bool hittable_list::bounding_box(double time0, double time1, aabb& output_box) const
{
	if (objects.empty())
		return false;

	aabb tmp_box;
	bool first = false;

	for (const auto& obj : objects)
	{
		if (obj.get()->bounding_box(time0, time1, tmp_box))
			output_box = first ? surrounding_box(output_box, tmp_box) : tmp_box;
		else
			return false;
		first = true;
	}
	return true;
}
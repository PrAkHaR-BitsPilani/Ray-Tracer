#pragma once

#include "rtweekend.h"
#include "hittable.h"

class flip_face : public hittable {

public:
	 flip_face(hittable* obj) : object(obj) {}

	 bool hit(
		const ray& r, float t_min, float t_max, hit_record& rec) const override {

		if (!object->hit(r, t_min, t_max, rec))
			return false;

		rec.front_face = !rec.front_face;
		return true;
	}

	 bool bounding_box(float time0, float time1, AxisAllignedBoundingBox& output_box) const override {
		return object->bounding_box(time0, time1, output_box);
	}


public:
	hittable* object;
};
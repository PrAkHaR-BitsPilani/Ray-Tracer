#pragma once

#pragma once



#include "rtweekend.h"
#include "hittable_list.h"
#include "rect.h"

/// <summary>
/// A box object which can be placed in the scene.
/// </summary>
class box : public hittable {

public:
	 box() {}
	 box(glm::vec3 a, glm::vec3 b, material* mat_ptr);

	 virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;

	 virtual bool bounding_box(float time0, float time1, AxisAllignedBoundingBox& output_box) const override {
		output_box = AxisAllignedBoundingBox(minPoint, maxPoint);
		return true;
	}


public:
	glm::vec3 minPoint;
	glm::vec3 maxPoint;
	hittable_list sides;
};

 box::box(glm::vec3 a, glm::vec3 b, material* mat_ptr) {
	minPoint = a;
	maxPoint = b;

	sides.add(new xy_rect(a.x, b.x, a.y, b.y, b.z, mat_ptr));
	sides.add(new xy_rect(a.x, b.x, a.y, b.y, a.z, mat_ptr));
	sides.add(new xz_rect(a.x, b.x, a.z, b.z, b.y, mat_ptr));
	sides.add(new xz_rect(a.x, b.x, a.z, b.z, a.y, mat_ptr));
	sides.add(new yz_rect(a.y, b.y, a.z, b.z, b.x, mat_ptr));
	sides.add(new yz_rect(a.y, b.y, a.z, b.z, a.x, mat_ptr));


}

 bool box::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {

	return sides.hit(r, t_min, t_max, rec);
}
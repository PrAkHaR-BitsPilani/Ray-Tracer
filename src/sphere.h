#pragma once

#include "rtweekend.h"

#include "hittable.h"

/// <summary>
/// A sphere object which can be placed in the scene.
/// </summary>
class sphere : public hittable {
public:
    sphere()
        : center(glm::vec3(0.0f)), radius(1.0f), mat_ptr(nullptr) {};

    sphere(glm::vec3 cen, float r, material* m)
        : center(cen), radius(r), mat_ptr(m) {};

    virtual bool hit(
        const ray& r, float t_min, float t_max, hit_record& rec) const override;
    virtual bool bounding_box(
        float time0, float time1, AxisAllignedBoundingBox& outbox) const override;

public:
    glm::vec3 center;
    float radius;
    material* mat_ptr;
};


bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    glm::vec3 oc = r.orig - center;
    auto a = pow(glm::length(r.dir), 2);
    auto half_b = glm::dot(oc, r.dir);
    auto c = pow(glm::length(oc), 2) - radius*radius;

    auto discriminant = half_b*half_b - a * c;
    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    rec.t = root;
    rec.p = r.at(root);
    glm::vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mat_ptr;
    return true;
}

bool sphere::bounding_box(float time0, float time1, AxisAllignedBoundingBox& outbox)const {
    outbox = AxisAllignedBoundingBox(center - glm::vec3(radius), center + glm::vec3(radius));
    return true;
}
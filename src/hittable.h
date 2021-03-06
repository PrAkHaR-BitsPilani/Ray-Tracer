#pragma once

#include "rtweekend.h"
#include "AxisAllignedBoundingBox.h"


class material;

struct hit_record {
    glm::vec3 p;
    glm::vec3 normal;
    material* mat_ptr;
    float t;   
    float u;
    float v;
    bool front_face;

    inline void set_face_normal(const ray& r, const glm::vec3& outward_normal) {
        front_face = glm::dot(r.dir, outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

/// <summary>
/// An abstract class represting a object which can be placed in a scene.
/// </summary>
class hittable {
public:
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
    virtual bool bounding_box(float time0, float time1, AxisAllignedBoundingBox& outbox) const = 0;
};
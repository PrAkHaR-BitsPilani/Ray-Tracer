#pragma once

#include "rtweekend.h"

#include "hittable.h"

#include <memory>
#include <vector>


class hittable_list : public hittable {
public:
    hittable_list() {}
    hittable_list(hittable* object) { add(object); }

    void clear() { objects.clear(); }
    void add(hittable* object) { objects.push_back(object); }

    virtual bool hit(
        const ray& r, float t_min, float t_max, hit_record& rec) const override;
    virtual bool bounding_box(
        float time0, float time1, AxisAllignedBoundingBox& outbox) const override;

public:
    std::vector<hittable*> objects;
};


bool hittable_list::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    hit_record temp_rec;    
    auto hit_anything = false;
    auto closest_so_far = t_max;

    for (const auto& object : objects) {
        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

bool hittable_list::bounding_box(float time0, float time1, AxisAllignedBoundingBox& outbox)const {
    if (objects.empty())
        return false;
    AxisAllignedBoundingBox tempBox;
    bool first = true;
    for (auto object : objects)
    {
        if (!object->bounding_box(time0, time1, tempBox))return false;
        if (first)outbox = surroundingBox(outbox, tempBox), first = false;
        else outbox = tempBox;
    }
    return true;
}
#pragma once

#include "rtweekend.h"
#include "hittable.h"


struct hit_record;


class material {
public:
    virtual bool scatter(
        const ray& r_in, const hit_record& rec, glm::vec3& attenuation, ray& scattered
    ) const = 0;
};

class lambertian : public material {

public:
    lambertian(const glm::vec3& a) : albedo(a) {}


    virtual bool scatter(
        const ray& r_in, const hit_record& rec, glm::vec3& attenuation, ray& scattered)const override {

        glm::vec3 scatterDir = rec.normal + random_unit_vector();
        if (nearZero(scatterDir))
            scatterDir = rec.normal;
        scattered = ray(rec.p, scatterDir);
        attenuation = albedo;
        return true;
    }
    
public:
    glm::vec3 albedo;
};

class metal : public material {

public:
    metal(const glm::vec3& a , float f) : albedo(a), fuzz(f) {}

    virtual bool scatter(
        const ray& r_in, const hit_record& rec, glm::vec3& attenuation, ray& scattered)const override {
        glm::vec3 reflected = glm::normalize(glm::reflect(r_in.dir, rec.normal));
        scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
        attenuation = albedo;
        return glm::dot(scattered.dir, rec.normal) > 0;
    }

public:
    glm::vec3 albedo;
    float fuzz;
};
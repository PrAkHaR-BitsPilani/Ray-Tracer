#pragma once

#include "rtweekend.h"
#include "hittable.h"
#include "texture.h"


struct hit_record;


class material {
public:
    virtual bool scatter(
        const ray& r_in, const hit_record& rec, glm::vec3& attenuation, ray& scattered
    ) const = 0;
    virtual glm::vec3 emitted(const ray& in, const hit_record& rec, float u, float v, const glm::vec3& p)const
    {
        return glm::vec3(0);
    }
};

class lambertian : public material {

public:
    lambertian(const glm::vec3& a) : albedo(a) {}


    virtual bool scatter(
        const ray& r_in, const hit_record& rec, glm::vec3& attenuation, ray& scattered)const override {

        glm::vec3 scatterDir = rec.normal + random_in_unit_sphere();
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

class diffuse_light : public material {

public:
     diffuse_light(Texture* t) : emit(t) {}
     diffuse_light(glm::vec3 color) : emit(new solid_color(color)) {}

     bool scatter(
        const ray& r_in, const hit_record& rec, glm::vec3& attenuation, ray& scattered) const override {
        return false;
    }

     glm::vec3 emitted(const ray& in, const hit_record& rec, float u, float v, const glm::vec3& p)const override {
        if (rec.front_face)
            return emit->value(u, v, p);
        return glm::vec3(0);
    }

public:
    Texture* emit;
};

class alienMat : public material {
public:

     alienMat(const glm::vec3& a, float f ) : albedo(a), fuzz(f), emit(new solid_color(a)) {}

     bool scatter(
        const ray& r_in, const hit_record& rec, glm::vec3& attenuation, ray& scattered)const override {
        glm::vec3 reflected = glm::normalize(glm::reflect(r_in.dir, rec.normal));
        scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
        attenuation = albedo;
        return glm::dot(scattered.dir, rec.normal) > 0;
    }

     glm::vec3 emitted(const ray& in , const hit_record& rec, float u, float v, const glm::vec3& p)const override {
        return emit->value(u, v, p);
    }

public:
    glm::vec3 albedo;
    float fuzz;
    Texture* emit;
};
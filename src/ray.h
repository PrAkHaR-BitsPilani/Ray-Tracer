#pragma once


#include "glm/glm.hpp"


class ray {
public:

    ray() : orig(glm::vec3(0.0f)) , dir(glm::vec3(0.0f)) , tm(0.0f) {}

    ray(const glm::vec3& origin, const glm::vec3& direction)
        : orig(origin), dir(direction), tm(0)
    {}

    ray(const glm::vec3& origin, const glm::vec3& direction, float time)
        : orig(origin), dir(direction), tm(time)
    {}

    glm::vec3 at(float t) const {
        return orig + t * dir;
    }

public:
    glm::vec3 orig;
    glm::vec3 dir;
    float tm;
};
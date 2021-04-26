#pragma once

#include "rtweekend.h"

/// <summary>
/// Simple camera which can be positioned anywhere.
/// </summary>
class camera {
public:
    camera(glm::vec3 cameraPos , glm::vec3 target , glm::vec3 worldUp , float vfov , float aspect_ratio) {
        
        auto theta = glm::radians(vfov);
        auto h = tan(theta / 2);
        auto viewport_height = 2.0f * h;
        auto viewport_width = aspect_ratio * viewport_height;

        auto n = glm::normalize(target - cameraPos);
        auto u = glm::cross(n, glm::normalize(worldUp));
        auto v = glm::cross(u, n);
        

        origin = cameraPos;
        horizontal = viewport_width * u;
        vertical = viewport_height * v;
        lower_left_corner = origin - horizontal / 2.0f - vertical / 2.0f + n ;
    }
    /// <summary>
    /// Returns the ray starting from the camera and ending at a particular pixel on he projection window.
    /// </summary>
    ray get_ray(float s, float t) const {
        return ray(origin, lower_left_corner + s * horizontal + t * vertical - origin);
    }

private:
    glm::vec3 origin;
    glm::vec3 lower_left_corner;
    glm::vec3 horizontal;
    glm::vec3 vertical;
};
#include "rtweekend.h"

#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include "BVH_Node.h"
#include "rect.h"
#include "box.h"
#include "flip_face.h"
#include "transform.h"

#include "stb_image/stb_image.h"
#include "stb_image/stb_image_write.h"

#include <iostream>
#include <vector>


glm::vec3 ray_color(const ray& r, const BVH_Node& root , int depth)
{

    if (depth <= 0)
        return glm::vec3(0.0f);

    hit_record rec;
    if (root.hit(r, 0.001, infinity, rec))
    {
        ray scattered;
        glm::vec3 attenuation;
        glm::vec3 emitted = rec.mat_ptr->emitted(r , rec , rec.u, rec.v, rec.p);
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        {
            return emitted + attenuation * ray_color(scattered, root, depth - 1);
        }
        return emitted;

    }
    return glm::vec3(0);
    /*glm::vec3 res;
    glm::vec3 dir = glm::normalize(r.dir);
    auto t = 0.5f * (dir.y + 1.0f);
    res = (1.0f - t) * glm::vec3(1.0f) + t * glm::vec3(0.5f, 0.7f, 1.0f);
    return res;*/
}

glm::vec3 ray_color(const ray& r, const hittable_list& root, int depth)
{

    if (depth <= 0)
        return glm::vec3(0.0f);

    hit_record rec;
    if (root.hit(r, 0.001, infinity, rec))
    {
        ray scattered;
        glm::vec3 attenuation;
        glm::vec3 emitted = rec.mat_ptr->emitted(r, rec, rec.u, rec.v, rec.p);
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        {
            return emitted + attenuation * ray_color(scattered, root, depth - 1);
        }
        return emitted;

    }
    //return glm::vec3(0);
    glm::vec3 res;
    glm::vec3 dir = glm::normalize(r.dir);
    auto t = 0.5f * (dir.y + 1.0f);
    res = (1.0f - t) * glm::vec3(1.0f) + t * glm::vec3(0.5f, 0.7f, 1.0f);
    return res;
}

hittable_list cornellBox() {
    hittable_list world;

    auto red = new lambertian(glm::vec3(0.65, 0.05, 0.05));
    auto green = new lambertian(glm::vec3(0.12, 0.45, 0.15));
    auto light = new diffuse_light(glm::vec3(15));
    auto white = new lambertian(glm::vec3(0.73));
    auto white_metal = new metal(glm::vec3(1),0);

    world.add(new yz_rect(0, 555, 0, 555, 555, green));
    world.add(new yz_rect(0, 555, 0, 555, 0, red));
    world.add(new flip_face(new xz_rect(213, 343, 227, 332, 545, light)));
    /*d_list[i++] = new xz_rect(10, 110, 445, 545, 545, light);
    d_list[i++] = new xz_rect(445, 545, 10, 110, 554, light);
    d_list[i++] = new xz_rect(445, 545, 445, 545, 554, light);*/
    world.add(new xz_rect(0, 555, 0, 555, 0, white));
    world.add(new xz_rect(0, 555, 0, 555, 555, white));
    world.add(new xy_rect(0, 555, 0, 555, 555, white));

    //d_list[i++] = new sphere(glm::vec3(50, 10, 30), 10, white);

    world.add(new Translate(
                new RotateY(
                    new box(glm::vec3(0, 0, 0), glm::vec3(165, 165, 165), white),-18),glm::vec3(130,0,65)));
    world.add(new Translate(
                new RotateY(
                    new box(glm::vec3(0, 0, 0), glm::vec3(165, 330, 165), white), 15), glm::vec3(265,0,295)));

    return world;

}

hittable_list simpleScene()
{
    hittable_list world;
    auto light = new diffuse_light(glm::vec3(15));
    auto light2 = new diffuse_light(glm::vec3(15,0,0));
    auto light3 = new diffuse_light(glm::vec3(15, 15, 0));
    world.add(new xy_rect(-5, 5, -5, 5, 0, light));
    world.add(new xz_rect(-5, 5, -5, 5, 0, light2));
    world.add(new yz_rect(-5, 5, -5, 5, 0, light3));
    return world;
  
}

hittable_list random_scene() {
    hittable_list world;

    auto ground_material = new lambertian(glm::vec3(0.5, 0.5, 0.5));
    world.add(new sphere(glm::vec3(0,-1000,0), 1000, ground_material));

    int limit = 3;

    for (int a = -limit; a < limit; a++) {
        for (int b = -limit; b < limit; b++) {
            auto choose_mat = random_float();
            glm::vec3 center(a + 0.9*random_float(), 0.2, b + 0.9*random_float());

            if ((center - glm::vec3(4, 0.2, 0)).length() > 0.9) {
                material* sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = randomColor() * randomColor();
                    sphere_material = new lambertian(albedo);
                    world.add(new sphere(center, 0.2, sphere_material));
                } else {
                    // metal
                    auto albedo = randomColor();
                    auto fuzz = random_float(0, 0.5);
                    sphere_material = new metal(albedo, fuzz);
                    world.add(new sphere(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material2 = new diffuse_light(glm::vec3(15));
    world.add(new sphere(glm::vec3(0, 1, 0), 1.0, material2));

    /*auto material3 = new metal(glm::vec3(0.7, 0.6, 0.5), 0.0);
    world.add(new sphere(glm::vec3(4, 1, 0), 1.0, material3));
    world.add(new sphere(glm::vec3(6, 1, 0), 1.0, material3));
    world.add(new sphere(glm::vec3(8, 1, 0), 1.0, material3));
    world.add(new sphere(glm::vec3(10, 1, 0), 1.0, material3));
    world.add(new sphere(glm::vec3(12, 1, 0), 1.0, material3));*/

    return world;
}

int  main()
{

    //Image
    const auto aspectRatio = 1;// 3840.0f / 2160.0f;
    const int imgWidth = 400;
    const int imgHeight = (int)(imgWidth/aspectRatio);
    const int number_of_samples = 10;
    const int maxDepth = 5;

    //World
    hittable_list world = cornellBox();

    BVH_Node root(world.objects,0,world.objects.size(), 0, 1);
    

    //Camera
    //camera cam(glm::vec3(15, 10, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), 45, aspectRatio);
    camera cam(glm::vec3(278, 278, -800), glm::vec3(278, 278, 0), glm::vec3(0, 1, 0), 40, aspectRatio);

    auto start = clock();
    

    std::vector<unsigned char> img;

    for (int j = imgHeight - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < imgWidth; ++i) {

            glm::vec3 pixel_color(0.0f);
            for (int s = 0; s < number_of_samples; s++)
            {
                auto u = float(i + random_float()) / (imgWidth - 1);
                auto v = float(j + random_float()) / (imgHeight - 1);
                ray r = cam.get_ray(u, v);
                glm::vec3 colorAdd = ray_color(r, root, maxDepth);
                if (colorAdd.r != colorAdd.r)colorAdd.r = 0;
                if (colorAdd.g != colorAdd.g)colorAdd.g = 0;
                if (colorAdd.b != colorAdd.b)colorAdd.b = 0;
                pixel_color += colorAdd;
            }
            img.push_back(255 * clamp(sqrt(pixel_color.r/number_of_samples), 0, 0.999f));
            img.push_back(255 * clamp(sqrt(pixel_color.g/number_of_samples), 0, 0.999f));
            img.push_back(255 * clamp(sqrt(pixel_color.b/number_of_samples), 0, 0.999f));

        }
    }

    int result = stbi_write_png("res/images/output.png", imgWidth, imgHeight, 3, img.data(), imgWidth * 3);
    std::cerr << result << "\n";
    std::cerr << "\nDone.\n";

    auto end = clock();

    std::cerr << "Time Elapsed " << ((double)(end - start)) / CLOCKS_PER_SEC << "s\n";

    return 0;

}
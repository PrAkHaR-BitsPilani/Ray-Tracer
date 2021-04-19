#pragma once

#include <stb_image/stb_image_write.h>

#include <iostream>
#include <vector>

int createJPG(const char* fileName, int width , int height, int channels , std::vector<unsigned char> img, int quality)
{
    int result = stbi_write_jpg(fileName, width, height, channels, img.data(), quality);
    return result;
}
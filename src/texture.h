#include "rtweekend.h"

class Texture {
public:
	 virtual glm::vec3 value(float u, float v, const glm::vec3& p)const = 0;
};

class solid_color : public Texture {

public:
	 solid_color()
		:color(glm::vec3(0)) {}
	 solid_color(glm::vec3 customColor)
		: color(customColor) {}
	 glm::vec3 value(float u, float v, const glm::vec3& p)const override;

private:
	glm::vec3 color;
};

glm::vec3 solid_color::value(float u, float v, const glm::vec3& p)const {
	return color;
}
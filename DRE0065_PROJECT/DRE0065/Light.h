#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include "Subject.h"

using namespace glm;

class Light : public Subject 
{
public:
    Light(const vec3& position, const vec3& color, float intensity);
    vec3 getPosition() const;
    vec3 getColor() const;
    float getIntensity() const;
    void setPosition(const vec3& position);
    void setColor(const vec3& color);
    void setIntensity(float intensity);

private:
    vec3 position;
    vec3 color;
    float intensity;
};
#endif
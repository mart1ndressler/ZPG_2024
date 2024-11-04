#include "Light.h"

Light::Light(const vec3& position, const vec3& color, float intensity): position(position), color(color), intensity(intensity) {}

vec3 Light::getPosition() const {return position;}
vec3 Light::getColor() const {return color;}
float Light::getIntensity() const {return intensity;}

void Light::setPosition(const vec3& position) 
{
    this->position = position;
    notifyObservers();
}

void Light::setColor(const vec3& color) 
{
    this->color = color;
    notifyObservers();
}

void Light::setIntensity(float intensity)
{
    this->intensity = intensity;
    notifyObservers();
}
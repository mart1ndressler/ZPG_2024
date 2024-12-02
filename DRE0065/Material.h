#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/glm.hpp>
#include <random>

using namespace glm;
using namespace std;

class Material 
{
public:
	Material();
    Material(const vec3& ambient, const vec3& diffuse, const vec3& specular, float shininess);

    const vec3& getAmbient() const;
    const vec3& getDiffuse() const;
    const vec3& getSpecular() const;
    float getShininess() const;

    void setAmbient(const vec3& ambient);
    void setDiffuse(const vec3& diffuse);
    void setSpecular(const vec3& specular);
    void setShininess(float shininess);

private:
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
#endif
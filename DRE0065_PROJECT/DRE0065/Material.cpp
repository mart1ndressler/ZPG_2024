#include "Material.h"

Material::Material() {}
Material::Material(const vec3& ambient, const vec3& diffuse, const vec3& specular, float shininess): ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess) {}

const vec3& Material::getAmbient() const {return ambient;}
const vec3& Material::getDiffuse() const {return diffuse;}
const vec3& Material::getSpecular() const {return specular;}
float Material::getShininess() const {return shininess;}

void Material::setAmbient(const vec3& ambient) {this->ambient = ambient;}
void Material::setDiffuse(const vec3& diffuse) {this->diffuse = diffuse;}
void Material::setSpecular(const vec3& specular) {this->specular = specular;}
void Material::setShininess(float shininess) {this->shininess = shininess;}
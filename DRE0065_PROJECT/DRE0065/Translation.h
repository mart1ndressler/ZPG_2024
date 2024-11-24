#ifndef TRANSLATION_H
#define TRANSLATION_H

#include "Transformation_Component.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

class Translation : public TransformationComponent 
{
public:
    Translation(const vec3& translation);
    void apply(mat4& matrix) const override;

private:
    vec3 translation;
};
#endif
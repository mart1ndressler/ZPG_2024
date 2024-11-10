#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <random>
#include <chrono>
#include <string>
#include "Subject.h"
#include "ShaderProgram.h"
#include "Transformation_Composite.h"

using namespace glm;

class Light : public Subject 
{
public:
    Light(const vec3& position, const vec3& color, float intensity);
    vec3 getPosition() const;
    vec3 getColor() const;
    float getIntensity() const;
    TransformationComposite* getTransformation() const;

    void setPosition(const vec3& position);
    void setColor(const vec3& color);
    void setIntensity(float intensity);
    void setTransformation(TransformationComposite* transformation);
    
    static void setLightUniforms(ShaderProgram* shaderProgram, const vector<Light*>& lights);
    static void updateLightMovement(float deltaTime, vector<Light*>& lights, vector<vec3>& directions, vector<float>& times);

private:
    vec3 position;
    vec3 color;
    float intensity;
    TransformationComposite* transformation = NULL;
};
#endif
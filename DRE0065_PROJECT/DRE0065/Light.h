#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include <random>
#include <chrono>
#include <string>
#include "Subject.h"
#include "ShaderProgram.h"
#include "Translation.h"
#include "Transformation_Composite.h"

using namespace glm;

class Light : public Subject 
{
public:
	enum LightType {DIRECTIONAL, POINT, SPOTLIGHT};
    Light(const vec3& position, const vec3& color, float intensity);

    vec3 getPosition() const;
    vec3 getColor() const;
    float getIntensity() const;
	vec3 getDirection() const;
	LightType getLightType() const;
    float getInnerCutOff() const;
    float getOuterCutOff() const;

    TransformationComposite* getTransformation() const;

    void setPosition(const vec3& position);
    void setColor(const vec3& color);
    void setIntensity(float intensity);
	void setDirection(const vec3& direction);
	void setLightType(LightType type);
    void setCutOff(float innerCutOff, float outerCutOff);

    void setTransformation(TransformationComposite* transformation);
    
    static void setLightUniforms(ShaderProgram* shaderProgram, const vector<Light*>& lights);
    static void updateLightMovement(float deltaTime, vector<Light*>& lights, vector<vec3>& directions, vector<float>& times);
    vector<Light*> generatePointLights(int numLights, float minDistance, const vector<Light*>& existingLights);

private:
    vec3 position;
    vec3 color;
    float intensity;
	vec3 direction;
    float innerCutOff;
    float outerCutOff;
	LightType type;
    TransformationComposite* transformation = NULL;
};
#endif
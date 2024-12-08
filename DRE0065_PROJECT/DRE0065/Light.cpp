#include "Light.h"

Light::Light(const vec3& position, const vec3& color, float intensity) : position(position), color(color), intensity(intensity), transformation(NULL), type(POINT), direction(vec3(0.0f)), innerCutOff(0.0f), outerCutOff(0.0f) {}

vec3 Light::getPosition() const {return position;}
vec3 Light::getColor() const {return color;}
float Light::getIntensity() const {return intensity;}
vec3 Light::getDirection() const {return direction;}
Light::LightType Light::getLightType() const {return type;}
float Light::getInnerCutOff() const {return innerCutOff;}
float Light::getOuterCutOff() const {return outerCutOff;}

TransformationComposite* Light::getTransformation() const {return transformation;}

void Light::setPosition(const vec3& position) {this->position = position;}
void Light::setColor(const vec3& color) {this->color = color;}
void Light::setIntensity(float intensity) {this->intensity = intensity;}
void Light::setDirection(const vec3& direction) {this->direction = direction;}
void Light::setLightType(LightType type) {this->type = type;}
void Light::setCutOff(float innerCutOff, float outerCutOff)
{
    this->innerCutOff = glm::cos(glm::radians(innerCutOff));
    this->outerCutOff = glm::cos(glm::radians(outerCutOff));
}

void Light::setTransformation(TransformationComposite* transformation) {this->transformation = transformation;}

void Light::setLightUniforms(ShaderProgram* shaderProgram, const vector<Light*>& lights)
{
    shaderProgram->use();
    GLuint programID = shaderProgram->getProgramID();

    glUniform1i(glGetUniformLocation(programID, "numLights"), static_cast<GLint>(lights.size()));
    for(int i=0; i < lights.size(); i++)
    {
        string posUniform = "lightPositions[" + to_string(i) + "]";
        string colorUniform = "lightColors[" + to_string(i) + "]";
        string directionUniform = "lightDirections[" + to_string(i) + "]";
        string isDirectionalUniform = "isDirectional[" + to_string(i) + "]";
        string innerCutOffUniform = "innerCutOff[" + to_string(i) + "]";
        string outerCutOffUniform = "outerCutOff[" + to_string(i) + "]";

        glUniform3fv(glGetUniformLocation(programID, posUniform.c_str()), 1, glm::value_ptr(lights[i]->getPosition()));
        glUniform3fv(glGetUniformLocation(programID, colorUniform.c_str()), 1, glm::value_ptr(lights[i]->getColor()));
        glUniform1i(glGetUniformLocation(programID, isDirectionalUniform.c_str()), lights[i]->getLightType() == Light::DIRECTIONAL);

        if(lights[i]->getLightType() == Light::DIRECTIONAL || lights[i]->getLightType() == Light::SPOTLIGHT) glUniform3fv(glGetUniformLocation(programID, directionUniform.c_str()), 1, glm::value_ptr(lights[i]->getDirection()));
        if(lights[i]->getLightType() == Light::SPOTLIGHT)
        {
            glUniform1f(glGetUniformLocation(programID, innerCutOffUniform.c_str()), lights[i]->getInnerCutOff());
            glUniform1f(glGetUniformLocation(programID, outerCutOffUniform.c_str()), lights[i]->getOuterCutOff());
        }
    }

    if(!lights.empty() && lights[0]->getLightType() == Light::DIRECTIONAL)
    {
        vec3 dirLightPlaneNormal = normalize(lights[0]->getDirection());
        float dirLightPlaneDistance = -2.0f;

        glUniform3fv(glGetUniformLocation(programID, "dirLightPlaneNormal"), 1, glm::value_ptr(dirLightPlaneNormal));
        glUniform1f(glGetUniformLocation(programID, "dirLightPlaneDistance"), dirLightPlaneDistance);
    }

    glUseProgram(0);
}

void Light::updateLightMovement(float deltaTime, vector<Light*>& lights, vector<vec3>& directions, vector<float>& times)
{
    static default_random_engine generator(static_cast<unsigned>(chrono::system_clock::now().time_since_epoch().count()));
    static uniform_real_distribution<float> translationDist(-0.8f, 0.8f);
    const float movementFactor = 0.05f;

    for(int i=0; i < lights.size(); i++)
    {
        times[i] += deltaTime;
        if(times[i] > 1.0f)
        {
            directions[i] = vec3(translationDist(generator), 0.0f, translationDist(generator));
            times[i] = 0.0f;
        }

        vec3 currentPos = lights[i]->getPosition();
        vec3 newPosition = currentPos + directions[i] * movementFactor;
        newPosition.x = clamp(newPosition.x, -5.0f, 5.0f);
        newPosition.z = clamp(newPosition.z, -15.0f, -5.0f);
        lights[i]->setPosition(newPosition);
    }
}

vector<Light*> Light::generatePointLights(int numLights, float minDistance, const vector<Light*>& existingLights)
{
    vector<Light*> newLights;
    default_random_engine generator_light(static_cast<unsigned>(chrono::system_clock::now().time_since_epoch().count()));
    uniform_real_distribution<float> posXDistance(-5.0f, 5.0f);
    uniform_real_distribution<float> posZDistance(-15.0f, -5.0f);
    uniform_real_distribution<float> colorDistance(0.5f, 1.0f);

    for(int i=0; i < numLights; i++)
    {
        vec3 lightPos;
        bool validPosition = false;

        while(!validPosition)
        {
            lightPos = vec3(posXDistance(generator_light), 2.0f, posZDistance(generator_light));
            validPosition = true;

            for(auto existingLight : existingLights)
            {
                float distance = glm::length(lightPos - existingLight->getPosition());
                if(distance < minDistance)
                {
                    validPosition = false;
                    break;
                }
            }

            for(auto newLight : newLights)
            {
                float distance = glm::length(lightPos - newLight->getPosition());
                if(distance < minDistance)
                {
                    validPosition = false;
                    break;
                }
            }
        }

        vec3 lightColor(colorDistance(generator_light), colorDistance(generator_light), colorDistance(generator_light));
        Light* newLight = new Light(lightPos, lightColor, 1.0f);
        newLight->setLightType(Light::POINT);

        TransformationComposite* lightTransform = new TransformationComposite();
        lightTransform->addComponent(new Translation(lightPos));
        newLight->setTransformation(lightTransform);
        newLights.push_back(newLight);
    }
    return newLights;
}
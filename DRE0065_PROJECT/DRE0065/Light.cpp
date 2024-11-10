#include "Light.h"

Light::Light(const vec3& position, const vec3& color, float intensity) : position(position), color(color), intensity(intensity), transformation(nullptr) {}

vec3 Light::getPosition() const {return position;}
vec3 Light::getColor() const {return color;}
float Light::getIntensity() const {return intensity;}
TransformationComposite* Light::getTransformation() const {return transformation;}

void Light::setPosition(const vec3& position) {this->position = position;}
void Light::setColor(const vec3& color) {this->color = color;}
void Light::setIntensity(float intensity) {this->intensity = intensity;}
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
        glUniform3fv(glGetUniformLocation(programID, posUniform.c_str()), 1, glm::value_ptr(lights[i]->getPosition()));
        glUniform3fv(glGetUniformLocation(programID, colorUniform.c_str()), 1, glm::value_ptr(lights[i]->getColor()));
    }
    glUseProgram(0);
}

void Light::updateLightMovement(float deltaTime, vector<Light*>& lights, vector<vec3>& directions, vector<float>& times)
{
    static default_random_engine generator(static_cast<unsigned>(chrono::system_clock::now().time_since_epoch().count()));
    static uniform_real_distribution<float> translationDist(-1.0f, 1.0f);
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
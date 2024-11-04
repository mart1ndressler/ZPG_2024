#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include "Observer.h"
#include "Shader.h"
#include "Camera.h"
#include "ShaderLoader.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

using namespace std;
using namespace glm;

class ShaderProgram : public Observer
{
public:
    ShaderProgram(const char* vertexPath, const char* fragmentPath, Camera* camera);
    void loadShaders(const string& vertexPath, const string& fragmentPath);
    void use() const;
    void setUniform(const string& name, const mat4& matrix) const;
    virtual void update() override;

private:
    void checkLinkingErrors(GLuint program) const;
    GLuint programID = 0;
    Camera* camera;
    ShaderLoader shaderLoader;
};
#endif
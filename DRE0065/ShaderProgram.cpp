#include "ShaderProgram.h"

using namespace std;
using namespace glm;

ShaderProgram::ShaderProgram(const char* vertexPath, const char* fragmentPath, Camera* camera) : camera(camera) 
{
    if(camera) camera->addObserver(this);
    loadShaders(vertexPath, fragmentPath);
}

void ShaderProgram::loadShaders(const string& vertexPath, const string& fragmentPath)
{
    shaderLoader = ShaderLoader(vertexPath.c_str(), fragmentPath.c_str(), &programID);
}

void ShaderProgram::use() const {glUseProgram(programID);}

void ShaderProgram::setUniform(const string& name, const mat4& matrix) const
{
    GLuint location = glGetUniformLocation(programID, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(matrix));
}

void ShaderProgram::update()
{
    mat4 viewMatrix = camera->getViewMatrix();
    setUniform("viewMatrix", viewMatrix);
    cout << "ShaderProgram: View matrix updated!" << endl;
}

void ShaderProgram::checkLinkingErrors(GLuint program) const
{
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success)
    {
        GLchar infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
    }
}

GLuint ShaderProgram::getProgramID() const {return programID;}

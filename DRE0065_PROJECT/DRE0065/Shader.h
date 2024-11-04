#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

class Shader 
{
public:
    Shader(const char* shaderPath, GLenum shaderType);
    void use() const;
    GLuint shaderID;

private:
    string readShaderFile(const char* filePath);
    void checkCompilation(GLuint shader) const;
};
#endif
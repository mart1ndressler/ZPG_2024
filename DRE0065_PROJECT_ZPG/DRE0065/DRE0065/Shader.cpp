#include "Shader.h"

using namespace std;

Shader::Shader(const char* shaderPath, GLenum shaderType)
{
    string shaderCode = readShaderFile(shaderPath);
    const char* sCode = shaderCode.c_str();
    shaderID = glCreateShader(shaderType);
    glShaderSource(shaderID, 1, &sCode, NULL);
    glCompileShader(shaderID);
    checkCompilation(shaderID);
}

void Shader::use() const {glUseProgram(shaderID);}
string Shader::readShaderFile(const char* filePath)
{
    ifstream shaderFile;
    stringstream shaderStream;
    try
    {
        shaderFile.open(filePath);
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        return shaderStream.str();
    }
    catch(...)
    {
        cerr << "Error occurred during shader compilation." << endl;
        return "";
    }
}

void Shader::checkCompilation(GLuint shader) const
{
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        cerr << "ERROR: Shader compilation failed\n" << infoLog << endl;
    }
}
#ifndef MODEL_H
#define MODEL_H

#include <GL/glew.h>

class Model 
{
public:
    Model();
    ~Model();
    GLuint VAO;
    void setupModel(const float* vertices, int size, int vertexCount, bool useNormals);

private:
    GLuint VBO;
};
#endif
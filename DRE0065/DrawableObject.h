#ifndef DRAWABLEOBJECT_H
#define DRAWABLEOBJECT_H

#include "Model.h"
#include "ShaderProgram.h"
#include "ShaderLoader.h"
#include "Transformation_Composite.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class DrawableObject 
{
public:
    DrawableObject(Model* model, GLuint shaderID, int vertexCount, GLenum drawMode);
    GLuint getShaderID() const;
    void draw(const mat4& viewMatrix, const mat4& projectionMatrix);
    TransformationComposite transformationcom;
    GLuint shaderID;

private:
    Model* model;
    int vertexCount;
    GLenum drawMode;
};
#endif

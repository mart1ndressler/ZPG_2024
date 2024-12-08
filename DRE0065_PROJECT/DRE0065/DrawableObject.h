#ifndef DRAWABLEOBJECT_H
#define DRAWABLEOBJECT_H

#include "Model.h"
#include "ShaderProgram.h"
#include "Transformation_Composite.h"
#include "Material.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class DrawableObject
{
public:
    DrawableObject(Model* model, ShaderProgram* shaderProgram, int vertexCount, GLenum drawMode, bool useIndices = false);
    ShaderProgram* getShaderProgram() const;
    void draw(const mat4& viewMatrix, const mat4& projectionMatrix);
	int getID() const {return ID;}
	void setID(int ID) {this->ID = ID;}
    TransformationComposite transformationcom;
    Material material;

private:
    Model* model;
    ShaderProgram* shaderProgram;
    int vertexCount;
    GLenum drawMode;
    bool useIndices;
    int ID;
};
#endif
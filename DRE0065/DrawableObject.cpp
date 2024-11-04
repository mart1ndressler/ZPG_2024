#include "DrawableObject.h"

DrawableObject::DrawableObject(Model* model, GLuint shaderID, int vertexCount, GLenum drawMode)
    : model(model), shaderID(shaderID), vertexCount(vertexCount), drawMode(drawMode), transformationcom() {}

GLuint DrawableObject::getShaderID() const {return shaderID;}

void DrawableObject::draw(const mat4& viewMatrix, const mat4& projectionMatrix)
{
    glUseProgram(shaderID);

    GLuint viewLoc = glGetUniformLocation(shaderID, "viewMatrix");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    GLuint projectionLoc = glGetUniformLocation(shaderID, "projectionMatrix");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    mat4 modelMatrix = mat4(1.0f);
    transformationcom.apply(modelMatrix);
    GLuint modelLoc = glGetUniformLocation(shaderID, "modelMatrix");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mat3 normalMatrix = mat3(transpose(inverse(modelMatrix)));
    GLuint normalLoc = glGetUniformLocation(shaderID, "normalMatrix");
    glUniformMatrix3fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

    glBindVertexArray(model->VAO);
    glDrawArrays(drawMode, 0, vertexCount);
    glBindVertexArray(0);

    glUseProgram(0);
}
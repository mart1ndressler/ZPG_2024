#include "DrawableObject.h"

DrawableObject::DrawableObject(Model* model, ShaderProgram* shaderProgram, int vertexCount, GLenum drawMode) : model(model), shaderProgram(shaderProgram), vertexCount(vertexCount), drawMode(drawMode), transformationcom() {}

ShaderProgram* DrawableObject::getShaderProgram() const {return shaderProgram;}

void DrawableObject::draw(const mat4& viewMatrix, const mat4& projectionMatrix)
{
    shaderProgram->use();

    GLuint viewLoc = glGetUniformLocation(shaderProgram->getProgramID(), "viewMatrix");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    GLuint projectionLoc = glGetUniformLocation(shaderProgram->getProgramID(), "projectionMatrix");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    mat4 modelMatrix = mat4(1.0f);
    transformationcom.apply(modelMatrix);
    GLuint modelLoc = glGetUniformLocation(shaderProgram->getProgramID(), "modelMatrix");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mat3 normalMatrix = mat3(transpose(inverse(modelMatrix)));
    GLuint normalLoc = glGetUniformLocation(shaderProgram->getProgramID(), "normalMatrix");
    glUniformMatrix3fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

    glBindVertexArray(model->VAO);
    glDrawArrays(drawMode, 0, vertexCount);
    glBindVertexArray(0);

    glUseProgram(0);
}
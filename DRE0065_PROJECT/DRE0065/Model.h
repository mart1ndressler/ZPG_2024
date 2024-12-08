#ifndef MODEL_H
#define MODEL_H

#include <GL/glew.h>
#include <string>
#include <assimp/Importer.hpp>
#include <iostream>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/mesh.h>
#include <assimp/material.h>
#include <assimp/vector3.h>
#include <vector>

using namespace std;

class Model
{
public:
    Model();
    ~Model();

    GLuint VAO, VBO, IBO;
	unsigned int indicesCount;
	unsigned int vertexCount;
    void setupModel(const float* vertices, int size, int vertexCount, bool useNormals, bool useTexCoords = false);
    void load(const string& fileName);
	int getIndicesCount() const {return indicesCount;}
    int getVertexCount() const {return vertexCount;}
	
private:
};
#endif
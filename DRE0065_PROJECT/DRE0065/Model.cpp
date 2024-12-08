#include "Model.h"

struct Vertex
{
    float Position[3];
    float Normal[3];
    float Texture[2];
    float Tangent[3];
};

Model::Model() : VAO(0), VBO(0), IBO(0), indicesCount(0), vertexCount(0) {}

Model::~Model()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &IBO);
}

void Model::setupModel(const float* vertices, int size, int vertexCount, bool useNormals, bool useTexCoords)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    if(useNormals && useTexCoords)
    {
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)(3 * sizeof(float)));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)(6 * sizeof(float)));
    }
    else if(useNormals)
    {
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid*)0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid*)(3 * sizeof(float)));
    }
    else
    {
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid*)0);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Model::load(const string& fileName)
{
    Assimp::Importer importer;
    unsigned int importOptions = aiProcess_Triangulate | aiProcess_OptimizeMeshes | aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace;

    const aiScene* scene = importer.ReadFile(fileName, importOptions);
    if(scene)
    {
        for(unsigned int m = 0; m < scene->mNumMeshes; m++)
        {
            aiMesh* mesh = scene->mMeshes[m];
            Vertex* pVertices = new Vertex[mesh->mNumVertices];
            memset(pVertices, 0, sizeof(Vertex) * mesh->mNumVertices);

            vertexCount = mesh->mNumVertices;

            for (unsigned int i = 0; i < mesh->mNumVertices; i++)
            {
                if(mesh->HasPositions()) 
                {
                    pVertices[i].Position[0] = mesh->mVertices[i].x;
                    pVertices[i].Position[1] = mesh->mVertices[i].y;
                    pVertices[i].Position[2] = mesh->mVertices[i].z;
                }

                if(mesh->HasNormals()) 
                {
                    pVertices[i].Normal[0] = mesh->mNormals[i].x;
                    pVertices[i].Normal[1] = mesh->mNormals[i].y;
                    pVertices[i].Normal[2] = mesh->mNormals[i].z;
                }

                if(mesh->HasTextureCoords(0)) 
                {
                    pVertices[i].Texture[0] = mesh->mTextureCoords[0][i].x;
                    pVertices[i].Texture[1] = mesh->mTextureCoords[0][i].y;
                }

                if(mesh->HasTangentsAndBitangents()) 
                {
                    pVertices[i].Tangent[0] = mesh->mTangents[i].x;
                    pVertices[i].Tangent[1] = mesh->mTangents[i].y;
                    pVertices[i].Tangent[2] = mesh->mTangents[i].z;
                }
            }

            if(mesh->HasFaces()) 
            {
                indicesCount = mesh->mNumFaces * 3;
                unsigned int* pIndices = new unsigned int[indicesCount];
                for(unsigned int i = 0; i < mesh->mNumFaces; i++) 
                {
                    pIndices[i * 3] = mesh->mFaces[i].mIndices[0];
                    pIndices[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
                    pIndices[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
                }

                glGenBuffers(1, &IBO);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indicesCount, pIndices, GL_STATIC_DRAW);
                if(mesh->HasFaces()) 
                {
                    indicesCount = mesh->mNumFaces * 3;
                    unsigned int* pIndices = new unsigned int[indicesCount];
                    for(unsigned int i = 0; i < mesh->mNumFaces; i++) 
                    {
                        if(mesh->mFaces[i].mNumIndices == 3) 
                        {
                            pIndices[i * 3] = mesh->mFaces[i].mIndices[0];
                            pIndices[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
                            pIndices[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
                        }
                    }

                    glGenBuffers(1, &IBO);
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
                    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indicesCount, pIndices, GL_STATIC_DRAW);
                    delete[] pIndices;
                }
                delete[] pIndices;
            }

            glGenVertexArrays(1, &VAO);
            GLuint VBO = 0;
            glGenBuffers(1, &VBO);

            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * mesh->mNumVertices, pVertices, GL_STATIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(0));
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(6 * sizeof(GLfloat)));

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);

            delete[] pVertices;
        }
    }
    else cerr << "Failed to load model: " << importer.GetErrorString() << endl;
}

#ifndef SCENE_H
#define SCENE_H

#include "DrawableObject.h"
#include "ShaderProgram.h"
#include "Camera.h"
#include "Light.h"
#include "Scale.h"
#include "Rotation.h"
#include "Translation.h"
#include "Transformation_Composite.h"
#include "Transformation_Component.h"
#include "skycube.h"
#include <vector>
#include <random>
#include <chrono>
#include <GLFW/glfw3.h>
#include <SOIL.h>
#include <ctime> 
#include <cstdlib>

using namespace std;
using namespace glm;

class Scene
{
public:
    Scene(GLFWwindow* window);
    ~Scene();
    void draw(float deltaTime);
    void currScene(int sceneNumber);
    void setUniformsAndDrawObjects(ShaderProgram* shaderProgram, const vec3& viewPos, const vec3& lightPos, const vec3& lightColor, const vec3& objectColor, const vector<DrawableObject*>& objects, const mat4& viewMatrix, const mat4& projectionMatrix, bool useLighting = true, bool setColor = true);
    void rotateTrees(float deltaTime);
    void updateProjectionMatrix(int width, int height);
	void handleInput(int key);
	void addTreeAtPosition(const vec3& position);
    mat4 getProjectionMatrix() const {return projectionMatrix;}
	GLuint loadTexture(const string& path);
    Camera* camera;
    Camera* camera2;

private:
	vector<DrawableObject*> scene1Objects, scene2Trees, scene2Ground, scene2Bushes, scene3Objects, scene4Objects, scene5Trees, scene5Ground, scene5Bushes, scene2Skybox, scene2Login, scene2Models;
    vector<Light*> scene2Lights, scene5Lights;
    int currentScene, currentShader;
    mat4 projectionMatrix;
    vector<vec3> lightDirections_scene2, lightDirections_scene5, bezierControlPoints;
    vector<float> timeSinceDirectionChange_scene2, timeSinceDirectionChange_scene5;

    ShaderProgram* triangleShader;
	ShaderProgram* groundShader;
	ShaderProgram* treeShader;
	ShaderProgram* bushShader;
	ShaderProgram* constantShader;
	ShaderProgram* lambertShader;
	ShaderProgram* phongShader;
	ShaderProgram* blinnShader;
    ShaderProgram* groundShaderNight;
    ShaderProgram* treeShaderNight;
    ShaderProgram* bushShaderNight;
	ShaderProgram* skyboxShader;
	ShaderProgram* loginShader;
	ShaderProgram* houseShader;

    void setupScene1();
    void setupScene2();
    void setupScene3();
    void setupScene4();
	void setupScene5();

	vec3 linearInterpolation(const vec3& start, const vec3& end, float t);
	vec3 bezierCurve(const vec3& p0, const vec3& p1, const vec3& p2, const vec3& p3, float t);

    GLFWwindow* window;
    Light* light_sc2;
    Light* light_sc3;
    Light* light_sc4;
    Light* light_sc5;
    GLuint grassTexture, cubemapTexture, houseTexture, grassTextureNight;
    bool skyboxFollowCamera = true;
    int movementMode;

    Model* sharedTreeModel;
    Model* sharedBushModel;

    void createTrees(int numTrees, float minDistance, ShaderProgram* shader, vector<DrawableObject*>& sceneTrees, vector<vec3>& objectPositions, default_random_engine& generator, uniform_real_distribution<float>& posXDist, uniform_real_distribution<float>& posZDist, uniform_real_distribution<float>& treeScaleDist, uniform_real_distribution<float>& rotationDist);
    void createBushes(int numBushes, float minDistance, ShaderProgram* shader, vector<DrawableObject*>& sceneBushes, vector<vec3>& objectPositions, default_random_engine& generator, uniform_real_distribution<float>& posXDist, uniform_real_distribution<float>& posZDist, uniform_real_distribution<float>& bushScaleDist, uniform_real_distribution<float>& rotationDist);
    int currID;
};
#endif
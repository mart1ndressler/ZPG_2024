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
#include <vector>
#include <random>
#include <chrono>
#include <GLFW/glfw3.h>

using namespace std;
using namespace glm;

class Scene
{
public:
    Scene(GLFWwindow* window);
    void draw(float deltaTime);
    void currScene(int sceneNumber);
    void setUniformsAndDrawObjects(ShaderProgram* shaderProgram, const vec3& viewPos, const vec3& lightPos, const vec3& lightColor, const vec3& objectColor, const vector<DrawableObject*>& objects, const mat4& viewMatrix, const mat4& projectionMatrix, bool useLighting = true, bool setColor = true);
    void rotateTrees(float deltaTime);
    void updateProjectionMatrix(int width, int height);
    Camera* camera;
    Camera* camera2;

private:
	vector<DrawableObject*> scene1Objects, scene2Trees, scene2Ground, scene2Bushes, scene3Objects, scene4Objects, scene5Trees, scene5Ground, scene5Bushes;
    vector<Light*> scene2Lights, scene5Lights;
    int currentScene, currentShader;
    mat4 projectionMatrix;
    vector<vec3> lightDirections_scene2, lightDirections_scene5;
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

    void setupScene1();
    void setupScene2();
    void setupScene3();
    void setupScene4();
	void setupScene5();

    GLFWwindow* window;
    Light* light_sc2;
    Light* light_sc3;
    Light* light_sc4;
    Light* light_sc5;
};
#endif
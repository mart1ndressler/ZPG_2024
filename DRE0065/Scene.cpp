#include "Scene.h"
#include "tree.h"
#include "bushes.h"
#include "sphere.h"
#include "suzi_smooth.h"
#include "plain.h"

Scene::Scene(GLFWwindow* window) : window(window), currentScene(1)
{
    camera = new Camera(vec3(0.0f, 2.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
    camera2 = new Camera(vec3(0.0f, 9.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), -90.0f, -90.0f);

    glfwSetWindowUserPointer(window, this); 
    setupScene1();
    setupScene2();
    setupScene3();
    setupScene4();
}

void Scene::setupScene1()
{
    float points[] =
    {
    0.0f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
   -0.5f, -0.5f, 0.0f
    };

    ShaderLoader shaderLoader;
    triangleShaderID = shaderLoader.loadShader("vertex_shader.glsl", "fragment_shader.glsl");

    Model* triangleModel = new Model();
    triangleModel->setupModel(points, sizeof(points), 3, false);

    DrawableObject* triangleObject = new DrawableObject(triangleModel, triangleShaderID, 3, GL_TRIANGLES);
    scene1Objects.push_back(triangleObject);

    ShaderProgram* triangleShader = new ShaderProgram("vertex_shader.glsl", "fragment_shader.glsl", camera);
}

void Scene::setupScene2()
{
    ShaderLoader shaderLoader;
    groundShaderID = shaderLoader.loadShader("vertex_shader1.glsl", "fragment_shader.glsl");
    treeShaderID = shaderLoader.loadShader("vertex_shader1.glsl", "fragment_shader1.glsl");
    bushShaderID = shaderLoader.loadShader("vertex_shader1.glsl", "fragment_shader1.glsl");

    light_sc2 = new Light(vec3(5.0f, 10.0f, -5.0f), vec3(1.0f, 1.0f, 1.0f), 1.0f);
    light_sc2->setPosition(vec3(-2.0f, 2.0f, 0.0f));

    Model* groundModel = new Model();
    groundModel->setupModel(plain, sizeof(plain), 6, true);

    DrawableObject* groundObject = new DrawableObject(groundModel, groundShaderID, 6, GL_TRIANGLES);

    TransformationComposite* groundTransform = new TransformationComposite();
    groundTransform->addComponent(new Scale(vec3(6.0f, 0.0f, 6.0f)));
    groundTransform->addComponent(new Translation(vec3(0.0f, -1.0f, -1.6f)));
    groundObject->transformationcom = *groundTransform;
    scene2Ground.push_back(groundObject);

    default_random_engine generator;
    uniform_real_distribution<float> posXDist(-5.0f, 5.0f), posZDist(-15.0f, -5.0f);
    uniform_real_distribution<float> treeScaleDist(0.05f, 0.1f), bushScaleDist(0.2f, 0.35f);
    uniform_real_distribution<float> rotationDist(0.0f, 360.0f);
    int numTrees = 300, numBushes = 300;
    float minDistance = 0.2f;
    vector<vec3> objectPositions;

    for(int i = 0; i < numTrees; i++)
    {
        bool validPosition = false;
        float posX, posZ;
        while(!validPosition)
        {
            posX = posXDist(generator);
            posZ = posZDist(generator);
            validPosition = true;

            for(int j = 0; j < objectPositions.size(); j++)
            {
                if(distance(vec3(posX, 0.0f, posZ), objectPositions[j]) < minDistance)
                {
                    validPosition = false;
                    break;
                }
            }
        }

        Model* treeModel = new Model();
        treeModel->setupModel(tree, sizeof(tree), 92814, true);
        DrawableObject* treeObject = new DrawableObject(treeModel, treeShaderID, 92814, GL_TRIANGLES);

        float randomScale = treeScaleDist(generator);
        float randomRotation = glm::radians(rotationDist(generator));

        TransformationComposite* treeTransform = new TransformationComposite();
        treeTransform->addComponent(new Translation(vec3(posX, 0.0f, posZ)));
        treeTransform->addComponent(new Rotation(randomRotation, vec3(0.0f, 1.0f, 0.0f)));
        treeTransform->addComponent(new Scale(vec3(randomScale, randomScale, randomScale)));
        treeObject->transformationcom = *treeTransform;

        scene2Trees.push_back(treeObject);
        objectPositions.push_back(vec3(posX, 0.0f, posZ));
    }

    for(int i = 0; i < numBushes; i++)
    {
        bool validPosition = false;
        float posX, posZ;
        while(!validPosition)
        {
            posX = posXDist(generator);
            posZ = posZDist(generator);
            validPosition = true;

            for(int j = 0; j < objectPositions.size(); j++)
            {
                if(distance(vec3(posX, 0.0f, posZ), objectPositions[j]) < minDistance)
                {
                    validPosition = false;
                    break;
                }
            }
        }

        Model* bushModel = new Model();
        bushModel->setupModel(bushes, sizeof(bushes), 8730, true);

        DrawableObject* bushObject = new DrawableObject(bushModel, bushShaderID, 8730, GL_TRIANGLES);

        float randomScale = bushScaleDist(generator);
        float randomRotation = glm::radians(rotationDist(generator));

        TransformationComposite* bushTransform = new TransformationComposite();
        bushTransform->addComponent(new Translation(vec3(posX, 0.0f, posZ)));
        bushTransform->addComponent(new Rotation(randomRotation, vec3(0.0f, 1.0f, 0.0f)));
        bushTransform->addComponent(new Scale(vec3(randomScale, randomScale, randomScale)));
        bushObject->transformationcom = *bushTransform;

        scene2Bushes.push_back(bushObject);
        objectPositions.push_back(vec3(posX, 0.0f, posZ));
    }
    ShaderProgram* groundShader = new ShaderProgram("vertex_shader1.glsl", "fragment_shader.glsl", camera);
    ShaderProgram* treeShader = new ShaderProgram("vertex_shader1.glsl", "fragment_shader1.glsl", camera);
    ShaderProgram* bushShader = new ShaderProgram("vertex_shader1.glsl", "fragment_shader1.glsl", camera);
}

void Scene::setupScene3()
{
    ShaderLoader shaderLoader;
    phongShaderID = shaderLoader.loadShader("phong_vertex_shader.glsl", "phong_fragment_shader.glsl");

    light_sc3 = new Light(vec3(0.0f, 3.0f, 0.0f), vec3(1.0f, 1.0f, 1.0f), 1.0f);
    //light_sc3->setPosition(vec3(-2.0f, 2.0f, 0.0f));
    light_sc3->setPosition(vec3(0.0f, 0.0f, 0.0f));

    vec3 positions[4] =
    {
        vec3(-2.0f, 0.0f, -2.0f),
        vec3(2.0f, 0.0f, -2.0f),
        vec3(-2.0f, 0.0f, 2.0f),
        vec3(2.0f, 0.0f, 2.0f)
    };

    for(int i = 0; i < 4; i++)
    {
        Model* sphereModel = new Model();
        sphereModel->setupModel(sphere, sizeof(sphere), 2880, true);

        DrawableObject* sphereObject = new DrawableObject(sphereModel, phongShaderID, 2880, GL_TRIANGLES);

        TransformationComposite* sphereTransform = new TransformationComposite();
        sphereTransform->addComponent(new Translation(positions[i]));
        sphereObject->transformationcom = *sphereTransform;

        scene3Objects.push_back(sphereObject);
    }
    ShaderProgram* phongShader = new ShaderProgram("phong_vertex_shader.glsl", "phong_fragment_shader.glsl", camera2);
}

void Scene::setupScene4()
{
    ShaderLoader shaderLoader;
    constantShaderID = shaderLoader.loadShader("constant_vertex_shader.glsl", "constant_fragment_shader.glsl");
    lambertShaderID = shaderLoader.loadShader("lambert_vertex_shader.glsl", "lambert_fragment_shader.glsl");
    blinnShaderID = shaderLoader.loadShader("blinn_vertex_shader.glsl", "blinn_fragment_shader.glsl");

    light_sc4 = new Light(vec3(0.0f, 3.0f, 0.0f), vec3(1.0f, 1.0f, 1.0f), 1.0f);
    //light_sc4->setPosition(vec3(-2.0f, 2.0f, 0.0f));
    light_sc4->setPosition(vec3(0.0f, 0.0f, 0.0f));

    vec3 positions[4] = 
    {
        vec3(-2.0f, 0.0f, -2.0f),
        vec3(2.0f, 0.0f, -2.0f),
        vec3(-2.0f, 0.0f, 2.0f),
        vec3(2.0f, 0.0f, 2.0f)
    };

    GLuint shaders[4] = {constantShaderID, lambertShaderID, phongShaderID, blinnShaderID};
    for(int i = 0; i < 4; i++)
    {
        Model* model = new Model();
        if(i < 2) model->setupModel(suziSmooth, sizeof(suziSmooth), 2904, true);
        else model->setupModel(sphere, sizeof(sphere), 2880, true);

        DrawableObject* object = new DrawableObject(model, shaders[i], (i < 2 ? 2904 : 2880), GL_TRIANGLES);
        TransformationComposite* objTransform = new TransformationComposite();
        objTransform->addComponent(new Translation(positions[i]));
        object->transformationcom = *objTransform;
        scene4Objects.push_back(object);
    }
    ShaderProgram* constantShader = new ShaderProgram("constant_vertex_shader.glsl", "constant_fragment_shader.glsl", camera2);
    ShaderProgram* lambertShader = new ShaderProgram("lambert_vertex_shader.glsl", "lambert_fragment_shader.glsl", camera2);
    ShaderProgram* blinnShader = new ShaderProgram("blinn_vertex_shader.glsl", "blinn_fragment_shader.glsl", camera2);
}

void Scene::currScene(int sceneNumber)
{
    currentScene = sceneNumber;
    if(currentScene == 1 || currentScene == 2) camera->resetPosition(vec3(0.0f, 2.0f, 1.0f), -90.0f, 0.0f);
    else if(currentScene == 3 || currentScene == 4) camera2->resetPosition(vec3(0.0f, 9.0f, 0.0f), -90.0f, -90.0f);
}

void Scene::setUniformsAndDrawObjects(GLuint shaderID, const vec3& viewPos, const vec3& lightPos, const vec3& lightColor, const vec3& objectColor, const vector<DrawableObject*>& objects, const mat4& viewMatrix, const mat4& projectionMatrix, bool useLighting, bool setColor)
{
    glUseProgram(shaderID);
    if(useLighting)
    {
        GLuint viewPosLoc = glGetUniformLocation(shaderID, "viewPos");
        GLuint lightPosLoc = glGetUniformLocation(shaderID, "lightPosition");
        GLuint lightColorLoc = glGetUniformLocation(shaderID, "lightColor");
        glUniform3fv(viewPosLoc, 1, glm::value_ptr(viewPos));
        glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));
        glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));
    }
    if(setColor)
    {
        GLuint objectColorLoc = glGetUniformLocation(shaderID, "objectColor");
        glUniform3fv(objectColorLoc, 1, glm::value_ptr(objectColor));
    }

    for(auto obj : objects) obj->draw(viewMatrix, projectionMatrix);
    glUseProgram(0);
}

void Scene::draw(float deltaTime)
{
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera->processKeyboard(0, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera->processKeyboard(1, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera->processKeyboard(2, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera->processKeyboard(3, deltaTime);

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera2->processKeyboard(0, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera2->processKeyboard(1, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera2->processKeyboard(2, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera2->processKeyboard(3, deltaTime);

    mat4 view = camera->getViewMatrix(), view1 = camera2->getViewMatrix();
    mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    if(currentScene == 1) setUniformsAndDrawObjects(triangleShaderID, vec3(0.0f), vec3(0.0f), vec3(0.0f), vec3(0.0f, 0.0f, 0.0f), scene1Objects, view, projection, false, false);
    else if(currentScene == 2)
    {
        vec3 lightPos2 = light_sc2->getPosition();
        vec3 lightColor2 = light_sc2->getColor();

        setUniformsAndDrawObjects(groundShaderID, camera->position, lightPos2, lightColor2, vec3(0.0f, 0.0f, 0.0f), scene2Ground, view, projection, true, false);
        setUniformsAndDrawObjects(treeShaderID, camera->position, lightPos2, lightColor2, vec3(0.6f, 0.3f, 0.1f), scene2Trees, view, projection, true, true);
        setUniformsAndDrawObjects(bushShaderID, camera->position, lightPos2, lightColor2, vec3(0.1f, 0.3f, 0.1f), scene2Bushes, view, projection, true, true);
    }
    else if(currentScene == 3)
    {
        vec3 lightPos3 = light_sc3->getPosition();
        vec3 lightColor3 = light_sc3->getColor();

        setUniformsAndDrawObjects(phongShaderID, camera2->position, lightPos3, lightColor3, vec3(0.2f, 0.2f, 0.2f), scene3Objects, view1, projection, true, true);
    }
    else if(currentScene == 4)
    {
        vec3 lightPos4 = light_sc4->getPosition();
        vec3 lightColor4 = light_sc4->getColor();

        for(auto obj : scene4Objects)
        {
            GLuint shaderID = obj->getShaderID();
            setUniformsAndDrawObjects(shaderID, camera2->position, lightPos4, lightColor4, vec3(0.2f, 0.2f, 0.2f), {obj}, view1, projection, true, true);
        }
    }
}
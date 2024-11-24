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
    projectionMatrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    glfwSetWindowUserPointer(window, this);
    setupScene1();
    setupScene2();
    setupScene3();
    setupScene4();
    setupScene5();
}

void Scene::setupScene1()
{
    float points[] =
    {
        0.0f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
       -0.5f, -0.5f, 0.0f
    };

    triangleShader = new ShaderProgram("vertex_shader.glsl", "fragment_shader.glsl", camera);
    triangleShader->use();

    Model* triangleModel = new Model();
    triangleModel->setupModel(points, sizeof(points), 3, false, false);

    DrawableObject* triangleObject = new DrawableObject(triangleModel, triangleShader, 3, GL_TRIANGLES);
    scene1Objects.push_back(triangleObject);
}

void Scene::setupScene2()
{
	groundShader = new ShaderProgram("texture_vertex.glsl", "texture_fragment.glsl", camera);
    treeShader = new ShaderProgram("vertex_shader1.glsl", "day_fragment_shader.glsl", camera);
    bushShader = new ShaderProgram("vertex_shader1.glsl", "day_fragment_shader.glsl", camera);
    skyboxShader = new ShaderProgram("skybox_vertex.glsl", "skybox_fragment.glsl", camera);

    light_sc2 = new Light(vec3(5.0f, 10.0f, -5.0f), vec3(1.0f, 1.0f, 1.0f), 1.0f);
    light_sc2->setPosition(vec3(-2.0f, 2.0f, 0.0f));

    int numLights = 5;
    float minDistanceLight = 3.5f;
    default_random_engine generator_light(static_cast<unsigned>(chrono::system_clock::now().time_since_epoch().count()));
    uniform_real_distribution<float> posXDistance(-5.0f, 5.0f), posZDistance(-15.0f, -5.0f);
    uniform_real_distribution<float> colorDistance(0.5f, 1.0f);
    for(int i=0; i < numLights; i++)
    {
        vec3 lightPos;
        bool validPosition = false;

        while(!validPosition)
        {
            lightPos = vec3(posXDistance(generator_light), 2.0f, posZDistance(generator_light));
            validPosition = true;

            for(auto existingLight : scene2Lights)
            {
                float distance = glm::length(lightPos - existingLight->getPosition());
                if(distance < minDistanceLight)
                {
                    validPosition = false;
                    break;
                }
            }
        }
        vec3 lightColor(colorDistance(generator_light), colorDistance(generator_light), colorDistance(generator_light));
        Light* newLight = new Light(lightPos, lightColor, 1.0f);

        TransformationComposite* lightTransform = new TransformationComposite();
        lightTransform->addComponent(new Translation(lightPos));
        newLight->setTransformation(lightTransform);
        scene2Lights.push_back(newLight);
    }

    grassTexture = SOIL_load_OGL_texture("grass_zpg.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
    if(!grassTexture) cerr << "Failed to load texture: grass_zpg.jpg" << endl;
    else cerr << "Texture grass_zpg.jpg loaded successfully, ID: " << grassTexture << endl;

    cubemapTexture = SOIL_load_OGL_cubemap("posx.jpg", "negx.jpg", "posy.jpg", "negy.jpg", "posz.jpg", "negz.jpg", SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
    if(!cubemapTexture) cerr << "Failed to load cubemap textures!" << endl;
    else cerr << "Cubemap textures loaded successfully, ID: " << cubemapTexture << endl;

    Model* skyboxModel = new Model();
    skyboxModel->setupModel(skycube, sizeof(skycube), 36, false, false);

    DrawableObject* skyboxObject = new DrawableObject(skyboxModel, skyboxShader, 36, GL_TRIANGLES);
    scene2Skybox.push_back(skyboxObject);

    glBindTexture(GL_TEXTURE_2D, grassTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    Model* groundModel = new Model();
    groundModel->setupModel(plain, sizeof(plain), 6, true, true);
    DrawableObject* groundObject = new DrawableObject(groundModel, groundShader, 6, GL_TRIANGLES);

    TransformationComposite* groundTransform = new TransformationComposite();
    groundTransform->addComponent(new Scale(vec3(6.0f, 0.0f, 6.0f)));
    groundTransform->addComponent(new Translation(vec3(0.0f, -1.0f, -1.6f)));
    groundObject->transformationcom = *groundTransform;
    scene2Ground.push_back(groundObject);

    default_random_engine generator;
    uniform_real_distribution<float> posXDist(-5.0f, 5.0f), posZDist(-15.0f, -5.0f);
    uniform_real_distribution<float> treeScaleDist(0.05f, 0.1f), bushScaleDist(0.2f, 0.35f);
    uniform_real_distribution<float> rotationDist(0.0f, 360.0f);
    //int numTrees = 200, numBushes = 50;
    int numTrees = 300, numBushes = 300;
    float minDistance = 0.2f;
    vector<vec3> objectPositions;

    int rotationInterval = 30;
    for(int i=0; i < numTrees; i++)
    {
        bool validPosition = false;
        float posX, posZ;
        while(!validPosition)
        {
            posX = posXDist(generator);
            posZ = posZDist(generator);
            validPosition = true;
            for(int j=0; j < objectPositions.size(); j++)
            {
                if(distance(vec3(posX, 0.0f, posZ), objectPositions[j]) < minDistance)
                {
                    validPosition = false;
                    break;
                }
            }
        }

        Model* treeModel = new Model();
        treeModel->setupModel(tree, sizeof(tree), 92814, true, false);
        DrawableObject* treeObject = new DrawableObject(treeModel, treeShader, 92814, GL_TRIANGLES);
        treeObject->material = Material(vec3(0.05f, 0.05f, 0.05f), vec3(0.1f, 0.3f, 0.1f), vec3(0.1f, 0.1f, 0.1f), 32.0f);

        float randomScale = treeScaleDist(generator);
        float randomRotation = glm::radians(rotationDist(generator));

        TransformationComposite* treeTransform = new TransformationComposite();
        treeTransform->addComponent(new Translation(vec3(posX, 0.0f, posZ)));
        treeTransform->addComponent(new Rotation(randomRotation, vec3(0.0f, 1.0f, 0.0f)));
        treeTransform->addComponent(new Scale(vec3(randomScale, randomScale, randomScale)));

        if(i % rotationInterval == 0)
        {
            treeObject->transformationcom = *treeTransform;
            scene2Trees.push_back(treeObject);
        }
        else
        {
            treeObject->transformationcom = *treeTransform;
            scene2Trees.push_back(treeObject);
        }
        objectPositions.push_back(vec3(posX, 0.0f, posZ));
    }

    for(int i=0; i < numBushes; i++)
    {
        bool validPosition = false;
        float posX, posZ;
        while(!validPosition)
        {
            posX = posXDist(generator);
            posZ = posZDist(generator);
            validPosition = true;

            for(int j=0; j < objectPositions.size(); j++)
            {
                if(distance(vec3(posX, 0.0f, posZ), objectPositions[j]) < minDistance)
                {
                    validPosition = false;
                    break;
                }
            }
        }

        Model* bushModel = new Model();
        bushModel->setupModel(bushes, sizeof(bushes), 8730, true, false);

        DrawableObject* bushObject = new DrawableObject(bushModel, bushShader, 8730, GL_TRIANGLES);
        bushObject->material = Material(vec3(0.08f, 0.18f, 0.08f), vec3(0.2f, 0.5f, 0.2f), vec3(0.05f, 0.05f, 0.05f), 64.0f);

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
}

void Scene::setupScene3()
{
    phongShader = new ShaderProgram("phong_vertex_shader.glsl", "phong_fragment_shader.glsl", camera2);

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

    Material sphereMaterial(vec3(0.2f, 0.2f, 0.2f), vec3(0.8f, 0.8f, 0.8f), vec3(1.0f, 1.0f, 1.0f), 64.0f);

    for(int i=0; i < 4; i++)
    {
        Model* sphereModel = new Model();
        sphereModel->setupModel(sphere, sizeof(sphere), 2880, true, false);

        DrawableObject* sphereObject = new DrawableObject(sphereModel, phongShader, 2880, GL_TRIANGLES);

        TransformationComposite* sphereTransform = new TransformationComposite();
        sphereTransform->addComponent(new Translation(positions[i]));
        sphereObject->transformationcom = *sphereTransform;

        sphereObject->material = sphereMaterial;
        scene3Objects.push_back(sphereObject);
    }
}

void Scene::setupScene4()
{
    constantShader = new ShaderProgram("constant_vertex_shader.glsl", "constant_fragment_shader.glsl", camera2);
    lambertShader = new ShaderProgram("lambert_vertex_shader.glsl", "lambert_fragment_shader.glsl", camera2);
    blinnShader = new ShaderProgram("blinn_vertex_shader.glsl", "blinn_fragment_shader.glsl", camera2);

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

    ShaderProgram* shaders[4] = { constantShader, lambertShader, phongShader, blinnShader };
    for(int i=0; i < 4; i++)
    {
        Model* model = new Model();
        if(i < 2) model->setupModel(suziSmooth, sizeof(suziSmooth), 2904, true, false);
        else model->setupModel(sphere, sizeof(sphere), 2880, true, false);

        DrawableObject* object = new DrawableObject(model, shaders[i], (i < 2 ? 2904 : 2880), GL_TRIANGLES);
        TransformationComposite* objTransform = new TransformationComposite();
        objTransform->addComponent(new Translation(positions[i]));
        object->transformationcom = *objTransform;

        if(i == 0) object->material = Material(vec3(0.7f, 0.7f, 0.7f), vec3(0.0f), vec3(0.0f), 1.0f);
        else if(i == 1) object->material = Material(vec3(0.0f), vec3(0.5f, 0.5f, 0.5f), vec3(0.0f), 16.0f);
        else if(i == 2) object->material = Material(vec3(0.2f, 0.2f, 0.2f), vec3(0.8f, 0.8f, 0.8f), vec3(1.0f, 1.0f, 1.0f), 64.0f);
        else if(i == 3) object->material = Material(vec3(0.0f), vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f), 32.0f);
        scene4Objects.push_back(object);
    }
}

void Scene::setupScene5()
{
	groundShaderNight = new ShaderProgram("texture_vertex.glsl", "texture_fragment.glsl", camera);
    treeShaderNight = new ShaderProgram("vertex_shader1.glsl", "night_fragment_shader.glsl", camera);
    bushShaderNight = new ShaderProgram("vertex_shader1.glsl", "night_fragment_shader.glsl", camera);

    light_sc5 = new Light(vec3(5.0f, 10.0f, -5.0f), vec3(1.0f, 1.0f, 1.0f), 1.0f);
    light_sc5->setPosition(vec3(-2.0f, 2.0f, 0.0f));

    /*
    Light* directionalLight = new Light(vec3(0.0f), vec3(0.4f, 0.4f, 0.6f), 1.0f);
    directionalLight->setDirection(vec3(-0.3f, -1.0f, -0.2f));
    directionalLight->setLightType(Light::DIRECTIONAL);
    scene5Lights.push_back(directionalLight);
    */

    /*
    for (int i = 0; i < 4; i++)
    {
        Light* spotlight = new Light(vec3(0.0f, 2.0f, 0.0f), vec3(1.0f, 1.0f, 1.0f), 20.0f);
        spotlight->setDirection(normalize(vec3(0.0f, 0.0f, 0.0f) - vec3(0.0f, 5.0f, 0.0f)));
        spotlight->setLightType(Light::SPOTLIGHT);
        spotlight->setCutOff(20.0f, 30.0f);
        scene5Lights.push_back(spotlight);
    }
    */

    Light* cameraSpotlight = new Light(camera->position, vec3(1.0f, 1.0f, 1.0f), 20.0f);
    cameraSpotlight->setDirection(camera->front);
    cameraSpotlight->setLightType(Light::SPOTLIGHT);
    cameraSpotlight->setCutOff(5.0f, 10.0f);
    scene5Lights.push_back(cameraSpotlight);

    Model* groundModel = new Model();
    groundModel->setupModel(plain, sizeof(plain), 6, true, true);

    DrawableObject* groundObject = new DrawableObject(groundModel, groundShaderNight, 6, GL_TRIANGLES);

    TransformationComposite* groundTransform = new TransformationComposite();
    groundTransform->addComponent(new Scale(vec3(6.0f, 0.0f, 6.0f)));
    groundTransform->addComponent(new Translation(vec3(0.0f, -1.0f, -1.6f)));
    groundObject->transformationcom = *groundTransform;
    scene5Ground.push_back(groundObject);

    default_random_engine generator;
    uniform_real_distribution<float> posXDist(-5.0f, 5.0f), posZDist(-15.0f, -5.0f);
    uniform_real_distribution<float> treeScaleDist(0.05f, 0.1f), bushScaleDist(0.2f, 0.35f);
    uniform_real_distribution<float> rotationDist(0.0f, 360.0f);
    int numTrees = 300, numBushes = 300;
    float minDistance = 0.2f;
    vector<vec3> objectPositions;

    int rotationInterval = 30;
    for(int i=0; i < numTrees; i++)
    {
        bool validPosition = false;
        float posX, posZ;
        while(!validPosition)
        {
            posX = posXDist(generator);
            posZ = posZDist(generator);
            validPosition = true;
            for(int j=0; j < objectPositions.size(); j++)
            {
                if(distance(vec3(posX, 0.0f, posZ), objectPositions[j]) < minDistance)
                {
                    validPosition = false;
                    break;
                }
            }
        }

        Model* treeModel = new Model();
        treeModel->setupModel(tree, sizeof(tree), 92814, true, false);
        DrawableObject* treeObject = new DrawableObject(treeModel, treeShaderNight, 92814, GL_TRIANGLES);
        treeObject->material = Material(vec3(0.05f, 0.05f, 0.05f), vec3(0.1f, 0.3f, 0.1f), vec3(0.1f, 0.1f, 0.1f), 32.0f);
        float randomScale = treeScaleDist(generator);
        float randomRotation = glm::radians(rotationDist(generator));

        TransformationComposite* treeTransform = new TransformationComposite();
        treeTransform->addComponent(new Translation(vec3(posX, 0.0f, posZ)));
        treeTransform->addComponent(new Rotation(randomRotation, vec3(0.0f, 1.0f, 0.0f)));
        treeTransform->addComponent(new Scale(vec3(randomScale, randomScale, randomScale)));

        if(i % rotationInterval == 0)
        {
            treeObject->transformationcom = *treeTransform;
            scene5Trees.push_back(treeObject);
        }
        else
        {
            treeObject->transformationcom = *treeTransform;
            scene5Trees.push_back(treeObject);
        }
        objectPositions.push_back(vec3(posX, 0.0f, posZ));
    }

    for(int i=0; i < numBushes; i++)
    {
        bool validPosition = false;
        float posX, posZ;
        while(!validPosition)
        {
            posX = posXDist(generator);
            posZ = posZDist(generator);
            validPosition = true;

            for(int j=0; j < objectPositions.size(); j++)
            {
                if(distance(vec3(posX, 0.0f, posZ), objectPositions[j]) < minDistance)
                {
                    validPosition = false;
                    break;
                }
            }
        }

        Model* bushModel = new Model();
        bushModel->setupModel(bushes, sizeof(bushes), 8730, true, false);

        DrawableObject* bushObject = new DrawableObject(bushModel, bushShaderNight, 8730, GL_TRIANGLES);
        bushObject->material = Material(vec3(0.08f, 0.18f, 0.08f), vec3(0.2f, 0.5f, 0.2f), vec3(0.05f, 0.05f, 0.05f), 64.0f);
        float randomScale = bushScaleDist(generator);
        float randomRotation = glm::radians(rotationDist(generator));

        TransformationComposite* bushTransform = new TransformationComposite();
        bushTransform->addComponent(new Translation(vec3(posX, 0.0f, posZ)));
        bushTransform->addComponent(new Rotation(randomRotation, vec3(0.0f, 1.0f, 0.0f)));
        bushTransform->addComponent(new Scale(vec3(randomScale, randomScale, randomScale)));
        bushObject->transformationcom = *bushTransform;

        scene5Bushes.push_back(bushObject);
        objectPositions.push_back(vec3(posX, 0.0f, posZ));
    }
}

void Scene::currScene(int sceneNumber)
{
    currentScene = sceneNumber;
    if(currentScene == 1 || currentScene == 2 || currentScene == 5) camera->resetPosition(vec3(0.0f, 2.0f, 1.0f), -90.0f, 0.0f);
    else if(currentScene == 3 || currentScene == 4) camera2->resetPosition(vec3(0.0f, 9.0f, 0.0f), -90.0f, -90.0f);
}

void Scene::setUniformsAndDrawObjects(ShaderProgram* shaderProgram, const vec3& viewPos, const vec3& lightPos, const vec3& lightColor, const vec3& objectColor, const vector<DrawableObject*>& objects, const mat4& viewMatrix, const mat4& projectionMatrix, bool useLighting, bool setColor)
{
    shaderProgram->use();
    GLuint programID = shaderProgram->getProgramID();

    if(useLighting)
    {
        GLuint viewPosLoc = glGetUniformLocation(programID, "viewPos");
        GLuint lightPosLoc = glGetUniformLocation(programID, "lightPosition");
        GLuint lightColorLoc = glGetUniformLocation(programID, "lightColor");
        glUniform3fv(viewPosLoc, 1, glm::value_ptr(viewPos));
        glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));
        glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));
    }

    for(auto obj : objects)
    {
        if(useLighting)
        {
            glUniform3fv(glGetUniformLocation(programID, "material.ambient"), 1, glm::value_ptr(obj->material.getAmbient()));
            glUniform3fv(glGetUniformLocation(programID, "material.diffuse"), 1, glm::value_ptr(obj->material.getDiffuse()));
            glUniform3fv(glGetUniformLocation(programID, "material.specular"), 1, glm::value_ptr(obj->material.getSpecular()));
            glUniform1f(glGetUniformLocation(programID, "material.shininess"), obj->material.getShininess());
        }
        if(setColor)
        {
            GLuint objectColorLoc = glGetUniformLocation(programID, "objectColor");
            glUniform3fv(objectColorLoc, 1, glm::value_ptr(objectColor));
        }

        obj->draw(viewMatrix, projectionMatrix);
    }
    glUseProgram(0);
}

void Scene::rotateTrees(float deltaTime)
{
    int rotationInterval = 30;
    static float rotationSpeed = glm::radians(20.0f);
    for(int i=0; i < scene2Trees.size(); i++)
    {
        if(i % rotationInterval == 0)
        {
            scene2Trees[i]->transformationcom.addComponent(new Rotation(rotationSpeed * deltaTime, vec3(0.0f, 1.0f, 0.0f)));
        }
    }
}

void Scene::updateProjectionMatrix(int width, int height)
{
    float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
    projectionMatrix = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
}

void Scene::handleInput(int key)
{
    static float lastKeyPressTime = 0.0f;
    float currentTime = glfwGetTime();

    if(key == GLFW_KEY_T && (currentTime - lastKeyPressTime >= 0.2f))
    {
        skyboxFollowCamera = !skyboxFollowCamera;
        lastKeyPressTime = currentTime;
        cout << "Skybox Follow Camera: " << (skyboxFollowCamera ? "Enabled" : "Disabled") << endl;
    }
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

    if(currentScene == 1)
    {
        triangleShader->use();
        setUniformsAndDrawObjects(triangleShader, vec3(0.0f), vec3(0.0f), vec3(0.0f), vec3(0.0f, 0.0f, 0.0f), scene1Objects, view, projectionMatrix, false, false);
        glUseProgram(0);
    }
    else if(currentScene == 2)
    {
        static vector<vec3> lightDirections_scene2(scene2Lights.size());
        static vector<float> timeSinceDirectionChange_scene2(scene2Lights.size(), 0.0f);
        vec3 lightPos2 = light_sc2->getPosition();
        vec3 lightColor2 = light_sc2->getColor();

        rotateTrees(deltaTime);
        Light::updateLightMovement(deltaTime, scene2Lights, lightDirections_scene2, timeSinceDirectionChange_scene2);

        treeShader->use();
        Light::setLightUniforms(treeShader, scene2Lights);
        setUniformsAndDrawObjects(treeShader, camera->position, lightPos2, lightColor2, vec3(0.2f, 0.4f, 0.2f), scene2Trees, view, projectionMatrix, true, true);

        bushShader->use();
        Light::setLightUniforms(bushShader, scene2Lights);
        setUniformsAndDrawObjects(bushShader, camera->position, lightPos2, lightColor2, vec3(0.3f, 0.5f, 0.3f), scene2Bushes, view, projectionMatrix, true, true);

        groundShader->use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, grassTexture);
        glUniform1i(glGetUniformLocation(groundShader->getProgramID(), "grassTexture"), 0);
        Light::setLightUniforms(groundShader, scene2Lights);
        setUniformsAndDrawObjects(groundShader, camera->position, lightPos2, lightColor2, vec3(0.0f, 0.0f, 0.0f), scene2Ground, view, projectionMatrix, true, false);

        mat4 skyboxView;
        if(skyboxFollowCamera) skyboxView = mat4(mat3(camera->getViewMatrix()));
        else skyboxView = camera->getViewMatrix();

        glDepthFunc(GL_LEQUAL);
        skyboxShader->use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glUniform1i(glGetUniformLocation(skyboxShader->getProgramID(), "skybox"), 0);

        setUniformsAndDrawObjects(skyboxShader, vec3(0.0f), vec3(0.0f), vec3(0.0f), vec3(0.0f), scene2Skybox, skyboxView, projectionMatrix, false, false);
        glDepthFunc(GL_LESS);
        glUseProgram(0);
    }
    else if(currentScene == 3)
    {
        vec3 lightPos3 = light_sc3->getPosition();
        vec3 lightColor3 = light_sc3->getColor();

        phongShader->use();
        setUniformsAndDrawObjects(phongShader, camera2->position, lightPos3, lightColor3, vec3(0.2f, 0.2f, 0.2f), scene3Objects, view1, projectionMatrix, true, true);
        glUseProgram(0);
    }
    else if(currentScene == 4)
    {
        vec3 lightPos4 = light_sc4->getPosition();
        vec3 lightColor4 = light_sc4->getColor();

        for(auto obj : scene4Objects)
        {
            ShaderProgram* currentShader = obj->getShaderProgram();
            if(currentShader)
            {
                currentShader->use();
                setUniformsAndDrawObjects(currentShader, camera2->position, lightPos4, lightColor4, vec3(0.2f, 0.2f, 0.2f), { obj }, view1, projectionMatrix, true, true);
                glUseProgram(0);
            }
        }
    }
    else if(currentScene == 5)
    {
        Light* cameraSpotlight = scene5Lights.back();
        cameraSpotlight->setPosition(camera->position);
        cameraSpotlight->setDirection(camera->front);

        static vector<vec3> lightDirections_scene5(scene5Lights.size());
        static vector<float> timeSinceDirectionChange_scene5(scene5Lights.size(), 0.0f);
        vec3 lightPos5 = light_sc5->getPosition();
        vec3 lightColor5 = light_sc5->getColor();

        Light::updateLightMovement(deltaTime, scene5Lights, lightDirections_scene5, timeSinceDirectionChange_scene5);

        treeShaderNight->use();
        Light::setLightUniforms(treeShaderNight, scene5Lights);
        setUniformsAndDrawObjects(treeShaderNight, camera->position, lightPos5, lightColor5, vec3(0.2f, 0.4f, 0.2f), scene5Trees, view, projectionMatrix, true, true);

        bushShaderNight->use();
        Light::setLightUniforms(bushShaderNight, scene5Lights);
        setUniformsAndDrawObjects(bushShaderNight, camera->position, lightPos5, lightColor5, vec3(0.3f, 0.5f, 0.3f), scene5Bushes, view, projectionMatrix, true, true);

        groundShaderNight->use();
        Light::setLightUniforms(groundShaderNight, scene5Lights);
        setUniformsAndDrawObjects(groundShaderNight, camera->position, lightPos5, lightColor5, vec3(0.0f, 0.0f, 0.0f), scene5Ground, view, projectionMatrix, true, false);

        glUseProgram(0);
    }
}
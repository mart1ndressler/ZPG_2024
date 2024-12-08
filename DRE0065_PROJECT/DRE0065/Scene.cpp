#include "Scene.h"
#include "tree.h"
#include "bushes.h"
#include "sphere.h"
#include "suzi_smooth.h"
#include "plain.h"

Scene::Scene(GLFWwindow* window) : window(window), currentScene(1), movementMode(0)
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

Scene::~Scene()
{
    delete triangleShader;
    delete groundShader;
    delete treeShader;
    delete bushShader;
    delete constantShader;
    delete lambertShader;
    delete phongShader;
    delete blinnShader;
    delete groundShaderNight;
    delete treeShaderNight;
    delete bushShaderNight;
    delete skyboxShader;
    delete loginShader;
    delete houseShader;

    delete sharedTreeModel;
    delete sharedBushModel;

    for(auto obj : scene1Objects) delete obj;
    for(auto obj : scene2Trees) delete obj;
    for(auto obj : scene2Ground) delete obj;
    for(auto obj : scene2Bushes) delete obj;
    for(auto obj : scene3Objects) delete obj;
    for(auto obj : scene4Objects) delete obj;
    for(auto obj : scene5Trees) delete obj;
    for(auto obj : scene5Ground) delete obj;
    for(auto obj : scene5Bushes) delete obj;
    for(auto obj : scene2Skybox) delete obj;
    for(auto obj : scene2Login) delete obj;
    for(auto obj : scene2Models) delete obj;

    for(auto light : scene2Lights) delete light;
    for(auto light : scene5Lights) delete light;

    glDeleteTextures(1, &grassTexture);
    glDeleteTextures(1, &cubemapTexture);
    glDeleteTextures(1, &houseTexture);
    glDeleteTextures(1, &grassTextureNight);
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

    DrawableObject* triangleObject = new DrawableObject(triangleModel, triangleShader, 3, GL_TRIANGLES, false);
    scene1Objects.push_back(triangleObject);
}

void Scene::setupScene2()
{
	groundShader = new ShaderProgram("texture_vertex.glsl", "texture_fragment.glsl", camera);
    treeShader = new ShaderProgram("vertex_shader1.glsl", "day_fragment_shader.glsl", camera);
    bushShader = new ShaderProgram("vertex_shader1.glsl", "day_fragment_shader.glsl", camera);
    skyboxShader = new ShaderProgram("skybox_vertex.glsl", "skybox_fragment.glsl", camera);
	loginShader = new ShaderProgram("model_vertex_shader.glsl", "model_fragment_shader.glsl", camera);
    houseShader = new ShaderProgram("model_vertex_shader.glsl", "model_fragment_shader.glsl", camera);
    
    light_sc2 = new Light(vec3(5.0f, 10.0f, -5.0f), vec3(1.0f, 1.0f, 1.0f), 1.0f);
    light_sc2->setPosition(vec3(-2.0f, 2.0f, 0.0f));
    scene2Lights.push_back(light_sc2);

	Model* loginModel = new Model();
	loginModel->load("login.obj");
	DrawableObject* loginObject = new DrawableObject(loginModel, loginShader, 0, GL_TRIANGLES, true);
	loginObject->setID(currID++);
	scene2Login.push_back(loginObject);

	Model* houseModel = new Model();
	houseModel->load("house.obj");
	DrawableObject* houseObject = new DrawableObject(houseModel, houseShader, 0, GL_TRIANGLES, true);
	houseObject->setID(currID++);

	houseTexture = loadTexture("house.png");

    TransformationComposite* houseTransform = new TransformationComposite();
    houseTransform->addComponent(new Scale(vec3(0.2f, 0.2f, 0.2f)));
    houseTransform->addComponent(new Translation(vec3(0.0f, 0.0f, -50.0f)));
    houseObject->transformationcom = *houseTransform;
	scene2Models.push_back(houseObject);

    int numLights = 4;
    float minDistanceLight = 3.5f;
    vector<Light*> generatedLights = light_sc2->generatePointLights(numLights, minDistanceLight, scene2Lights);
    for(auto light : generatedLights) scene2Lights.push_back(light);

	grassTexture = loadTexture("grass_zpg.jpg");

    cubemapTexture = SOIL_load_OGL_cubemap("posx.jpg", "negx.jpg", "posy.jpg", "negy.jpg", "posz.jpg", "negz.jpg", SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
    if(!cubemapTexture) cerr << "Failed to load cubemap textures!" << endl;
    else cerr << "Cubemap textures loaded successfully, ID: " << cubemapTexture << endl;

    Model* skyboxModel = new Model();
    skyboxModel->setupModel(skycube, sizeof(skycube), 36, false, false);

    DrawableObject* skyboxObject = new DrawableObject(skyboxModel, skyboxShader, 36, GL_TRIANGLES, false);
    scene2Skybox.push_back(skyboxObject);

    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    Model* groundModel = new Model();
    groundModel->setupModel(plain, sizeof(plain), 6, true, true);
    DrawableObject* groundObject = new DrawableObject(groundModel, groundShader, 6, GL_TRIANGLES, false);

    TransformationComposite* groundTransform = new TransformationComposite();
    groundTransform->addComponent(new Scale(vec3(6.0f, 0.0f, 6.0f)));
    groundTransform->addComponent(new Translation(vec3(0.0f, -1.0f, -1.6f)));
    groundObject->transformationcom = *groundTransform;
    scene2Ground.push_back(groundObject);

    default_random_engine generator;
    uniform_real_distribution<float> posXDist(-5.0f, 5.0f), posZDist(-15.0f, -5.0f);
    uniform_real_distribution<float> treeScaleDist(0.05f, 0.1f), bushScaleDist(0.2f, 0.35f);
    uniform_real_distribution<float> rotationDist(0.0f, 360.0f);
    int numTrees = 150, numBushes = 100;
    float minDistance = 0.2f;
    vector<vec3> objectPositions;

    sharedTreeModel = new Model();
    sharedTreeModel->setupModel(tree, sizeof(tree), 92814, true, false);

    sharedBushModel = new Model();
    sharedBushModel->setupModel(bushes, sizeof(bushes), 8730, true, false);

    createTrees(numTrees, minDistance, treeShader, scene2Trees, objectPositions, generator, posXDist, posZDist, treeScaleDist, rotationDist);
    createBushes(numBushes, minDistance, bushShader, scene2Bushes, objectPositions, generator, posXDist, posZDist, bushScaleDist, rotationDist);

    for(auto& treeObject : scene2Trees) treeObject->material = Material(vec3(0.05f, 0.05f, 0.05f), vec3(0.1f, 0.3f, 0.1f), vec3(0.1f, 0.1f, 0.1f), 32.0f);
    for(auto& bushObject : scene2Bushes) bushObject->material = Material(vec3(0.08f, 0.18f, 0.08f), vec3(0.2f, 0.5f, 0.2f), vec3(0.05f, 0.05f, 0.05f), 64.0f);
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

        DrawableObject* sphereObject = new DrawableObject(sphereModel, phongShader, 2880, GL_TRIANGLES, false);

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

    ShaderProgram* shaders[4] = {constantShader, lambertShader, phongShader, blinnShader};
    for(int i=0; i < 4; i++)
    {
        Model* model = new Model();
        if(i < 2) model->setupModel(suziSmooth, sizeof(suziSmooth), 2904, true, false);
        else model->setupModel(sphere, sizeof(sphere), 2880, true, false);

        DrawableObject* object = new DrawableObject(model, shaders[i], (i < 2 ? 2904 : 2880), GL_TRIANGLES, false);
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
  
    Light* directionalLight = new Light(vec3(0.0f), vec3(0.4f, 0.4f, 0.6f), 1.0f);
    directionalLight->setDirection(vec3(-0.3f, -1.0f, -0.2f));
    directionalLight->setLightType(Light::DIRECTIONAL);
    scene5Lights.push_back(directionalLight);

    Light* cameraSpotlight = new Light(camera->position, vec3(1.0f, 1.0f, 1.0f), 20.0f);
    cameraSpotlight->setDirection(camera->front);
    cameraSpotlight->setLightType(Light::SPOTLIGHT);
    cameraSpotlight->setCutOff(5.0f, 10.0f);
    scene5Lights.push_back(cameraSpotlight);

	grassTextureNight = loadTexture("grass_zpg.jpg");

    Model* groundModel = new Model();
    groundModel->setupModel(plain, sizeof(plain), 6, true, true);
    DrawableObject* groundObject = new DrawableObject(groundModel, groundShaderNight, 6, GL_TRIANGLES, false);

    TransformationComposite* groundTransform = new TransformationComposite();
    groundTransform->addComponent(new Scale(vec3(6.0f, 0.0f, 6.0f)));
    groundTransform->addComponent(new Translation(vec3(0.0f, -1.0f, -1.6f)));
    groundObject->transformationcom = *groundTransform;
    scene5Ground.push_back(groundObject);

    default_random_engine generator;
    uniform_real_distribution<float> posXDist(-5.5f, 5.5f), posZDist(-15.0f, -5.0f);
    uniform_real_distribution<float> treeScaleDist(0.05f, 0.1f), bushScaleDist(0.2f, 0.35f);
    uniform_real_distribution<float> rotationDist(0.0f, 360.0f);
    int numTrees = 300, numBushes = 300;
    float minDistance = 0.2f;
    vector<vec3> objectPositions;

	sharedTreeModel = new Model();
	sharedTreeModel->setupModel(tree, sizeof(tree), 92814, true, false);

	sharedBushModel = new Model();
	sharedBushModel->setupModel(bushes, sizeof(bushes), 8730, true, false);

	createTrees(numTrees, minDistance, treeShaderNight, scene5Trees, objectPositions, generator, posXDist, posZDist, treeScaleDist, rotationDist);
	createBushes(numBushes, minDistance, bushShaderNight, scene5Bushes, objectPositions, generator, posXDist, posZDist, bushScaleDist, rotationDist);

	for(auto& treeObject : scene5Trees) treeObject->material = Material(vec3(0.05f, 0.05f, 0.05f), vec3(0.1f, 0.3f, 0.1f), vec3(0.1f, 0.1f, 0.1f), 32.0f);
	for(auto& bushObject : scene5Bushes) bushObject->material = Material(vec3(0.08f, 0.18f, 0.08f), vec3(0.2f, 0.5f, 0.2f), vec3(0.05f, 0.05f, 0.05f), 64.0f);
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
    static double lastKeyPressTime = 0.0f;
    double currentTime = glfwGetTime();

    if(key == GLFW_KEY_T && (currentTime - lastKeyPressTime >= 0.2f))
    {
        skyboxFollowCamera = !skyboxFollowCamera;
        lastKeyPressTime = currentTime;
        cout << "Skybox Follow Camera: " << (skyboxFollowCamera ? "Enabled" : "Disabled") << endl;
    }

    if(key == GLFW_KEY_M && (currentTime - lastKeyPressTime >= 0.2f))
    {
        movementMode = (movementMode + 1) % 3;
        lastKeyPressTime = currentTime;
        cout << "Movement Mode: " << movementMode << endl;
    }
}

void Scene::addTreeAtPosition(const vec3& position)
{
    static int treeID = 1000;
    default_random_engine generator(static_cast<unsigned>(time(NULL)));
    uniform_real_distribution<float> scaleDist(0.05f, 0.1f);
    uniform_real_distribution<float> rotationDist(0.0f, 360.0f);

    Model* treeModel = new Model();
    treeModel->setupModel(tree, sizeof(tree), 92814, true, false);

    DrawableObject* treeObject = new DrawableObject(treeModel, treeShader, 92814, GL_TRIANGLES, false);
    treeObject->setID(treeID++);
    treeObject->material = Material(vec3(0.05f, 0.05f, 0.05f), vec3(0.1f, 0.3f, 0.1f), vec3(0.1f, 0.1f, 0.1f), 32.0f);

    float randomScale = scaleDist(generator);
    float randomRotation = glm::radians(rotationDist(generator));

    TransformationComposite* treeTransform = new TransformationComposite();
    treeTransform->addComponent(new Translation(position));
    treeTransform->addComponent(new Rotation(randomRotation, vec3(0.0f, 1.0f, 0.0f)));
    treeTransform->addComponent(new Scale(vec3(randomScale, randomScale, randomScale)));

    treeObject->transformationcom = *treeTransform;

    scene2Trees.push_back(treeObject);
    cout << "New tree added at position: ["<< position.x << ", " << position.y << ", " << position.z << "], "<< "Scale: " << randomScale << ", "<< "Rotation (Y): " << glm::degrees(randomRotation) << " degrees\n";
}

vec3 Scene::linearInterpolation(const vec3& start, const vec3& end, float t) {return (1 - t) * start + t * end;}

vec3 Scene::bezierCurve(const vec3& p0, const vec3& p1, const vec3& p2, const vec3& p3, float t) 
{
    float u = 1.0f - t;
    return (u * u * u) * p0 +
        (3 * u * u * t) * p1 +
        (3 * u * t * t) * p2 +
        (t * t * t) * p3;
}

GLuint Scene::loadTexture(const string& path)
{
    GLuint texture = SOIL_load_OGL_texture(path.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
    if(!texture) cerr << "Failed to load texture: " << path << endl;
    else cerr << "Texture " << path << " loaded successfully, ID: " << texture << endl;

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return texture;
}

void Scene::createTrees(int numTrees, float minDistance, ShaderProgram* shader, vector<DrawableObject*>& sceneTrees, vector<vec3>& objectPositions, default_random_engine& generator, uniform_real_distribution<float>& posXDist, uniform_real_distribution<float>& posZDist, uniform_real_distribution<float>& treeScaleDist, uniform_real_distribution<float>& rotationDist)
{
    for(int i=0; i < numTrees; i++)
    {
        bool validPosition = false;
        float posX, posZ;
        while(!validPosition)
        {
            posX = posXDist(generator);
            posZ = posZDist(generator);
            validPosition = true;
            for(const auto& pos : objectPositions)
            {
                if(distance(vec3(posX, 0.0f, posZ), pos) < minDistance)
                {
                    validPosition = false;
                    break;
                }
            }
        }

        DrawableObject* treeObject = new DrawableObject(sharedTreeModel, shader, 92814, GL_TRIANGLES, false);
        treeObject->setID(currID++);

        float randomScale = treeScaleDist(generator);
        float randomRotation = glm::radians(rotationDist(generator));

        TransformationComposite* treeTransform = new TransformationComposite();
        treeTransform->addComponent(new Translation(vec3(posX, 0.0f, posZ)));
        treeTransform->addComponent(new Rotation(randomRotation, vec3(0.0f, 1.0f, 0.0f)));
        treeTransform->addComponent(new Scale(vec3(randomScale, randomScale, randomScale)));

        treeObject->transformationcom = *treeTransform;
        sceneTrees.push_back(treeObject);
        objectPositions.push_back(vec3(posX, 0.0f, posZ));
    }
}

void Scene::createBushes(int numBushes, float minDistance, ShaderProgram* shader, vector<DrawableObject*>& sceneBushes, vector<vec3>& objectPositions, default_random_engine& generator, uniform_real_distribution<float>& posXDist, uniform_real_distribution<float>& posZDist, uniform_real_distribution<float>& bushScaleDist, uniform_real_distribution<float>& rotationDist)
{
    for(int i=0; i < numBushes; i++)
    {
        bool validPosition = false;
        float posX, posZ;
        while(!validPosition)
        {
            posX = posXDist(generator);
            posZ = posZDist(generator);
            validPosition = true;

            for(const auto& pos : objectPositions)
            {
                if(distance(vec3(posX, 0.0f, posZ), pos) < minDistance)
                {
                    validPosition = false;
                    break;
                }
            }
        }

        DrawableObject* bushObject = new DrawableObject(sharedBushModel, shader, 8730, GL_TRIANGLES, false);
        bushObject->setID(currID++);

        float randomScale = bushScaleDist(generator);
        float randomRotation = glm::radians(rotationDist(generator));

        TransformationComposite* bushTransform = new TransformationComposite();
        bushTransform->addComponent(new Translation(vec3(posX, 0.0f, posZ)));
        bushTransform->addComponent(new Rotation(randomRotation, vec3(0.0f, 1.0f, 0.0f)));
        bushTransform->addComponent(new Scale(vec3(randomScale, randomScale, randomScale)));
        bushObject->transformationcom = *bushTransform;

        sceneBushes.push_back(bushObject);
        objectPositions.push_back(vec3(posX, 0.0f, posZ));
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

		//rotateTrees(deltaTime);
        Light::updateLightMovement(deltaTime, scene2Lights, lightDirections_scene2, timeSinceDirectionChange_scene2);

        static vector<vec3> bezierControlPoints =
        {
            vec3(0.0f, 1.5f, -5.0f),
            vec3(2.0f, 3.0f, -7.0f),
            vec3(-2.0f, 2.0f, -10.0f),
            vec3(0.0f, 1.5f, -15.0f)
        };

        static float animationTime = 0.0f;
        static const float totalAnimationDuration = 5.0f;

        animationTime += deltaTime;
        float t = fmod(animationTime / totalAnimationDuration, 1.0f);

        vec3 newPosition;

        if(movementMode == 0) newPosition = linearInterpolation(bezierControlPoints[0], bezierControlPoints[3], t);
        else if(movementMode == 1) newPosition = bezierCurve(bezierControlPoints[0], bezierControlPoints[1], bezierControlPoints[2], bezierControlPoints[3],t);
        else if(movementMode == 2)
        {
            float segmentT = t * 3.0f;
            int segmentIndex = static_cast<int>(segmentT);
            float localT = segmentT - segmentIndex;
            newPosition = bezierCurve(bezierControlPoints[segmentIndex], bezierControlPoints[segmentIndex] + vec3(1.0f, 0.0f, -1.0f), bezierControlPoints[segmentIndex + 1] - vec3(1.0f, 0.0f, -1.0f), bezierControlPoints[segmentIndex + 1], localT);
        }

        if(!scene2Login.empty())
        {
            scene2Login[0]->transformationcom.clear();
            scene2Login[0]->transformationcom.addComponent(new Translation(newPosition));
            scene2Login[0]->transformationcom.addComponent(new Rotation(glm::radians(180.0f), vec3(0.0f, 1.0f, 0.0f)));
        }

        glEnable(GL_STENCIL_TEST);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

        treeShader->use();
        Light::setLightUniforms(treeShader, scene2Lights);
        for(DrawableObject* obj : scene2Trees) 
        {
            glStencilFunc(GL_ALWAYS, obj->getID(), 0xFF);
            obj->draw(view, projectionMatrix);
        }

        bushShader->use();
        Light::setLightUniforms(bushShader, scene2Lights);
        for(DrawableObject* obj : scene2Bushes) 
        {
            glStencilFunc(GL_ALWAYS, obj->getID(), 0xFF);
            obj->draw(view, projectionMatrix);
        }

        groundShader->use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, grassTexture);
        glUniform1i(glGetUniformLocation(groundShader->getProgramID(), "grassTexture"), 0);
        Light::setLightUniforms(groundShader, scene2Lights);
        for(DrawableObject* obj : scene2Ground) 
        {
            glStencilFunc(GL_ALWAYS, obj->getID(), 0xFF);
            obj->draw(view, projectionMatrix);
        }

        loginShader->use();
        Light::setLightUniforms(loginShader, scene2Lights);
        for(DrawableObject* obj : scene2Login)
        {
            glStencilFunc(GL_ALWAYS, obj->getID(), 0xFF);
            obj->draw(view, projectionMatrix);
        }

        houseShader->use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, houseTexture);
        glUniform1i(glGetUniformLocation(houseShader->getProgramID(), "houseTexture"), 0);
        Light::setLightUniforms(houseShader, scene2Lights);
        for(DrawableObject* obj : scene2Models)
        {
            glStencilFunc(GL_ALWAYS, obj->getID(), 0xFF);
            obj->draw(view, projectionMatrix);
        }

        glDisable(GL_STENCIL_TEST);

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

		loginShader->use();
		Light::setLightUniforms(loginShader, scene2Lights);
		setUniformsAndDrawObjects(loginShader, camera->position, lightPos2, lightColor2, vec3(0.0f, 0.0f, 0.0f), scene2Login, view, projectionMatrix, true, false);

		houseShader->use();
		glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, houseTexture);
		glUniform1i(glGetUniformLocation(houseShader->getProgramID(), "houseTexture"), 0);
        Light::setLightUniforms(houseShader, scene2Lights);
		setUniformsAndDrawObjects(houseShader, camera->position, lightPos2, lightColor2, vec3(0.0f, 0.0f, 0.0f), scene2Models, view, projectionMatrix, true, false);

        mat4 skyboxView;
        if(skyboxFollowCamera) skyboxView = mat4(mat3(camera->getViewMatrix()));
        else skyboxView = camera->getViewMatrix();

        glDepthFunc(GL_LEQUAL);
        skyboxShader->use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glUniform1i(glGetUniformLocation(skyboxShader->getProgramID(), "skybox"), 0);

        setUniformsAndDrawObjects(skyboxShader, camera->position, vec3(0.0f), vec3(0.0f), vec3(0.0f), scene2Skybox, skyboxView, projectionMatrix, false, false);
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
        glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, grassTextureNight);
		glUniform1i(glGetUniformLocation(groundShaderNight->getProgramID(), "grassTextureNight"), 0);
        Light::setLightUniforms(groundShaderNight, scene5Lights);
        setUniformsAndDrawObjects(groundShaderNight, camera->position, lightPos5, lightColor5, vec3(0.0f, 0.0f, 0.0f), scene5Ground, view, projectionMatrix, true, false);

        glUseProgram(0);
    }
}
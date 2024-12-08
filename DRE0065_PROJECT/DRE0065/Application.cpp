#include "Application.h"

float lastX = 400.0f, lastY = 300.0f;
bool firstMouse = true;
bool rightMousePressed = false;

Application::Application()
{
    initWindow();
    initOpenGL();
    scene = new Scene(window);
}

Application::~Application()
{
    delete scene;
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Application::initWindow()
{
    if(!glfwInit()) exit(EXIT_FAILURE);
    window = glfwCreateWindow(800, 600, "DRE0065 - PROJECT ZPG", nullptr, nullptr);
    if(!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Application::initOpenGL()
{
    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK)
    {
        cerr << "ERROR: Could not start GLEW\n";
        exit(EXIT_FAILURE);
    }
    glEnable(GL_DEPTH_TEST);

    printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
    printf("Using GLEW %s\n", glewGetString(GLEW_VERSION));
    printf("Vendor %s\n", glGetString(GL_VENDOR));
    printf("Renderer %s\n", glGetString(GL_RENDERER));
    printf("GLSL %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    int major, minor, revision;
    glfwGetVersion(&major, &minor, &revision);
    printf("Using GLFW %i.%i.%i\n", major, minor, revision);
    printf("---------------------------------------------------------------------------------------\n");
}

void Application::run()
{
    while(!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        static float lastFrame = 0.0f;
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        if(glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) scene->currScene(1);
        if(glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) scene->currScene(2);
        if(glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) scene->currScene(3);
        if(glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) scene->currScene(4);
        if(glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) scene->currScene(5);
		if(glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) scene->handleInput(GLFW_KEY_T);
        if(glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) scene->handleInput(GLFW_KEY_M);

        scene->draw(deltaTime);
        glFlush();
        glFinish();
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    Scene* scene = static_cast<Scene*>(glfwGetWindowUserPointer(window));
    if(!scene || !scene->camera || !scene->camera2) return;

    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        GLint x = static_cast<GLint>(xpos), y = static_cast<GLint>(ypos), newy = viewport[3] - y;

        GLuint objectID;
        glReadPixels(x, newy, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &objectID);

        GLfloat depth;
        glReadPixels(x, newy, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);

        vec3 screenPos = vec3((float)x, (float)newy, depth);
        mat4 viewMatrix = scene->camera->getViewMatrix();
        mat4 projectionMatrix = scene->getProjectionMatrix();
        vec4 viewportVec = vec4(viewport[0], viewport[1], viewport[2], viewport[3]);
        vec3 worldPos = unProject(screenPos, viewMatrix, projectionMatrix, viewportVec);

        if(objectID == 0) scene->addTreeAtPosition(worldPos);
        else cout << "Clicked on object with ID: " << objectID << " at position: [x: " << worldPos.x << ", y: " << worldPos.y << ", z: " << worldPos.z << "]\n";
    }

    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        rightMousePressed = true;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        if(firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        double xoffset = xpos - lastX, yoffset = lastY - ypos;
        lastX = xpos;
        lastY = ypos;

        scene->camera->processMouseMovement((float)xoffset, (float)yoffset);
        scene->camera2->processMouseMovement((float)xoffset, (float)yoffset);
    }
    else
    {
        rightMousePressed = false;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        firstMouse = true;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    Scene* scene = static_cast<Scene*>(glfwGetWindowUserPointer(window));
    if(scene) scene->updateProjectionMatrix(width, height);
    glViewport(0, 0, width, height);
}
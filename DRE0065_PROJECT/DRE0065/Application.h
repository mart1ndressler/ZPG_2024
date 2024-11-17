#ifndef APPLICATION_H
#define APPLICATION_H

#include "Scene.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

using namespace std;

class Application
{
public:
    Application();
    ~Application();
    void run();

private:
    GLFWwindow* window;
    Scene* scene;
    void initWindow();
    void initOpenGL();
};

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
#endif
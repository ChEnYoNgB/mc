#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "utils.h"
#include "camera.h"
#include "world.h"
#include "skybox.h"
#include "shadermanager.h"

#define WINDOWS_WIDTH 640
#define WINDOWS_HEIGHT 480
static double lastX = 320, lastY = 240;
static bool firstMouse = true;

void ProcessInput(GLFWwindow* window, Camera& camera, float deltaTime)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {     
        camera.MoveLeft(deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.MoveRight(deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.MoveForward(deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.MoveBack(deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        camera.MoveDown(deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {        
        camera.MoveUp(deltaTime);
    }
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    double xoffset = xpos - lastX;
    double yoffset = lastY - ypos; 
    lastX = xpos;
    lastY = ypos;

    auto* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));

    camera->MoveMouse(static_cast<float>(xoffset), static_cast<float>(yoffset));
}

GLFWwindow* InitWindows(int width, int height)
{
    GLFWwindow* window = nullptr;

    if (!glfwInit())
        return nullptr;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return nullptr;
    }
    return window;
}

void CompileShader(ShaderManager& shaders)
{
    shaders.Add("terrain",
    Utils::ReadTextFile("shaders/basic.vert"),
    Utils::ReadTextFile("shaders/basic.frag"),
    "./resource/image.png");
    shaders.Add("skybox",
    Utils::ReadTextFile("shaders/skybox.vert"),
    Utils::ReadTextFile("shaders/skybox.frag"));
}

int main(void)
{
    GLFWwindow* window = InitWindows(WINDOWS_WIDTH, WINDOWS_HEIGHT);

    if(!window)
        return -1;

    glm::vec3 worldPosition{0.0f, 0.0f, 0.0f};
    glm::vec3 forward{0.0f, 0.0f, -1.0f};
    glm::vec3 up{0.0f, 1.0f, 0.0f};
    float cameraSpeed{10.0f};
    Camera camera{worldPosition, forward, up, cameraSpeed, glm::radians(45.0f), static_cast<float>(WINDOWS_WIDTH) / WINDOWS_HEIGHT, 0.1f, 100.0f};
    {
        ShaderManager shaders;
        CompileShader(shaders);
        SkyBox skybox{};
        World world{123124512};
        float lastFrame = static_cast<float>(glfwGetTime());
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetWindowUserPointer(window, &camera);
        glfwSetCursorPosCallback(window, MouseCallback); 

        while (!glfwWindowShouldClose(window))
        {
            const float currentFrame = static_cast<float>(glfwGetTime());
            const float deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;
            std::cout << "fps: " << float(1.0 / deltaTime) << std::endl;
            ProcessInput(window, camera, deltaTime);
            glEnable(GL_DEPTH_TEST);
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            world.Update(camera);
            world.Draw(shaders, camera);
            glDepthFunc(GL_LEQUAL);
            skybox.Draw(shaders, camera);
            glDepthFunc(GL_LESS);
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}
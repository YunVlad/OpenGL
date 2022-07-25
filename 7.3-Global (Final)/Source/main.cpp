#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "shader.h"
#include "camera.h"
#include "objectsRender.h"


camera Camera(glm::vec3(-0.5f, 10.0f, -9.0f), glm::vec3(0.0f, 1.0f, 0.0f), 90.0f, -45.0f);
float lastX = 960;
float lastY = 540;
bool firstMouse = true;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    Camera.ProcessMouseMovement(xoffset, yoffset);
}

GLenum glCheckError_(const char* file, int line)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        std::string error;
        switch (errorCode)
        {
        case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
        case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
        case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
        case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
        case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
        case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
        case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        std::cout << error << " | " << file << " (" << line << ")" << std::endl;
    }
    return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)

void APIENTRY glDebugOutput(GLenum source,
    GLenum type,
    unsigned int id,
    GLenum severity,
    GLsizei length,
    const char* message,
    const void* userParam)
{
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " << message << std::endl;

    switch (source)
    {
    case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
    case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
    case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
    } 
    std::cout << std::endl;

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
    case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
    case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
    case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
    case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
    case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
    case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
    } 
    std::cout << std::endl;

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
    case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
    case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
    } 
    std::cout << std::endl;
    std::cout << std::endl;
}

int process()
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    window = glfwCreateWindow(1920, 1080, "OpenGL", NULL, NULL);
    glfwMakeContextCurrent(window);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    int flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    objectsRender render;

    std::vector<unsigned> goldTexture = render.loadPBRtextures("PBR/gold");
    std::vector<unsigned> marbleTexture = render.loadPBRtextures("PBR/marble");
    std::vector<unsigned> mirrorTexture = render.loadPBRtextures("PBR/metalG");
    std::vector<unsigned> ceramicTexture = render.loadPBRtextures("PBR/ñeramicG");

    std::vector<std::vector<unsigned>> texturePack;
    texturePack.push_back(mirrorTexture);
    texturePack.push_back(marbleTexture);
    texturePack.push_back(goldTexture);
    texturePack.push_back(ceramicTexture);

    render.addTextureToPack(goldTexture);
    render.addTextureToPack(marbleTexture);
    render.addTextureToPack(mirrorTexture);
    render.addTextureToPack(ceramicTexture);

    //Shaders configure

    shader lightShaders("Shaders/VertexLight", "Shaders/ColorLight");

    shader cubeShaders("Shaders/VertexStand", "Shaders/fCube");
    shader backShaders("Shaders/VertexBack", "Shaders/fBack");
    shader convShaders("Shaders/VertexStand", "Shaders/fConv");
    shader prefShaders("Shaders/VertexStand", "Shaders/fPrefilter");
    shader BRDFShaders("Shaders/VertexBRDF", "Shaders/fBRDF");
    shader PBRshaders("Shaders/VertexPBR", "Shaders/fPBRspecEnv");

    shader screenShaders("Shaders/VertexFrame","Shaders/fFrame");
    shader backEnvShaders("Shaders/VertexBack", "Shaders/fEnv");
    shader convEnvShaders("Shaders/VertexStand", "Shaders/fConv");
    shader prefEnvShaders("Shaders/VertexStand", "Shaders/fPrefilter");
    shader BRDFEnvShaders("Shaders/VertexBRDF", "Shaders/fBRDF");
    shader PBREnvshaders("Shaders/VertexPBR", "Shaders/fPBRspec");

    screenShaders.UseShaderProgramm();
    screenShaders.IntUniform("screenTexture", 1);

    glm::vec3 lightPos1 = glm::vec3(0.0f, 5.0f, 0.0f);
    glm::vec3 lightColor1 = glm::vec3(1.0f, 1.0f, 1.0f);
    int currLight = 1;

    std::vector<glm::vec3> currPos =
    { glm::vec3(0.0f,  0.0f, 0.0f),
      glm::vec3(4.0f,  0.0f, 0.0f), 
      glm::vec3(0.0f,  0.0f, 4.0f),
      glm::vec3(4.0f,  0.0f, 4.0f), };

    render.activePBRmaps(PBRshaders);
    render.activePBRmaps(PBREnvshaders);

    render.createSkybox(cubeShaders, backShaders, "PBR/env.hdr");
    render.IBLdiffuse(convShaders);
    render.IBLspec(prefShaders);
    render.IBLbrdf(BRDFShaders);

    glm::mat4 proj = glm::mat4(1.0f);
    proj = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);
    PBRshaders.UseShaderProgramm();
    PBRshaders.mat4Uniform("proj", proj);
    backShaders.UseShaderProgramm();
    backShaders.mat4Uniform("proj", proj);

    int scrWidth, scrHeight;
    glfwGetFramebufferSize(window, &scrWidth, &scrHeight);
    glViewport(0, 0, scrWidth, scrHeight);

    render.createMapSet(4);
    bool preRender = true;

    while (!glfwWindowShouldClose(window))
    {
        float time = glfwGetTime();

        Camera.setCameraSpeed(time);

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            Camera.ProcessKeyboardMovement(FORWARD);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            Camera.ProcessKeyboardMovement(BACKWARD);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            Camera.ProcessKeyboardMovement(LEFT);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            Camera.ProcessKeyboardMovement(RIGHT);

        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
            currLight = 1;

        if ((glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) && (currLight == 1))
            lightPos1.z += 0.1f;
        if ((glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) && (currLight == 1))
            lightPos1.z -= 0.1f;
        if ((glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) && (currLight == 1))
            lightPos1.x += 0.1f;
        if ((glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) && (currLight == 1))
            lightPos1.x -= 0.1f;
        if ((glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) && (currLight == 1))
            lightPos1.y += 0.1f;
        if ((glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) && (currLight == 1))
            lightPos1.y -= 0.1f;

        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
            lightColor1 = glm::vec3(1.0f, 0.1f, 0.1f);
        if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
            lightColor1 = glm::vec3(0.0f, 1.0f, 0.0f);
        if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
            lightColor1 = glm::vec3(0.0f, 0.0f, 1.0f);
        if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
            lightColor1 = glm::vec3(1.0f, 1.0f, 1.0f);

        glm::mat4 view;
        glm::vec3 viewPos;

        if (preRender)
        {
            PBRshaders.UseShaderProgramm();
            PBRshaders.vec3Uniform("lights[0].color", lightColor1);
            PBRshaders.vec3Uniform("lights[0].pos", lightPos1);
            PBRshaders.vec3Uniform("lights[0].atten", 1.0f, 0.045f, 0.0075f);
            PBRshaders.FloatUniform("lights[0].diff", 1.0f);
            PBRshaders.FloatUniform("lights[0].spec", 0.9f);

            render.objectPreRender(0, currPos, texturePack, 
                                      PBRshaders, backEnvShaders, 
                                      convEnvShaders, prefEnvShaders, BRDFEnvShaders);

            render.objectPreRender(1, currPos, texturePack,
                                      PBRshaders, backEnvShaders,
                                      convEnvShaders, prefEnvShaders, BRDFEnvShaders);

            render.objectPreRender(2, currPos, texturePack,
                                      PBRshaders, backEnvShaders,
                                      convEnvShaders, prefEnvShaders, BRDFEnvShaders);

            render.objectPreRender(3, currPos, texturePack,
                                      PBRshaders, backEnvShaders,
                                      convEnvShaders, prefEnvShaders, BRDFEnvShaders);

            
            render.shadowMapCreate();          
            preRender = false;
        }

        // Main render
        // PBR
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, scrWidth, scrHeight);

        view = Camera.mat4getView();
        viewPos = Camera.getCameraPos();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        PBREnvshaders.UseShaderProgramm();

        proj = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);

        PBREnvshaders.mat4Uniform("proj", proj);
        PBREnvshaders.mat4Uniform("view", view);
        PBREnvshaders.vec3Uniform("viewPos", viewPos);

        PBREnvshaders.vec3Uniform("lights[0].color", lightColor1);
        PBREnvshaders.vec3Uniform("lights[0].pos", lightPos1);
        PBREnvshaders.vec3Uniform("lights[0].atten", 1.0f, 0.045f, 0.0075f);
        PBREnvshaders.FloatUniform("lights[0].diff", 1.0f);
        PBREnvshaders.FloatUniform("lights[0].spec", 0.9f);

        render.objectIBLEnvsphere(PBREnvshaders, 0, currPos[0], mirrorTexture);
        render.objectIBLEnvsphere(PBREnvshaders, 1, currPos[1], marbleTexture);
        render.objectIBLEnvsphere(PBREnvshaders, 2, currPos[2], goldTexture);
        render.objectIBLEnvsphere(PBREnvshaders, 3, currPos[3], ceramicTexture);

        //Lighting render

        lightShaders.UseShaderProgramm();

        glm::mat4 lightModel1 = glm::mat4(1.0f);
        lightModel1 = glm::translate(lightModel1, lightPos1);
        lightModel1 = glm::scale(lightModel1, glm::vec3(0.05f));

        lightShaders.mat4Uniform("model", lightModel1);
        lightShaders.mat4Uniform("proj", proj);
        lightShaders.mat4Uniform("view", view);
        lightShaders.vec3Uniform("lightColor", lightColor1);

        backShaders.UseShaderProgramm();
        backShaders.mat4Uniform("view", view);
        backShaders.mat4Uniform("proj", proj);
        
        render.skyboxRender();


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

int main()
{
    process();
    return 0;
}
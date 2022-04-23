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

int process()
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(1920, 1080, "OpenGL", NULL, NULL);
    glfwMakeContextCurrent(window);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    objectsRender render;

    std::vector<unsigned> metalATexture = render.loadPBRtextures("PBR/metalA");
    std::vector<unsigned> marbleTexture = render.loadPBRtextures("PBR/marble");
    std::vector<unsigned> pavingTexture = render.loadPBRtextures("PBR/paving");
    std::vector<unsigned> metalBTexture = render.loadPBRtextures("PBR/metalB");

    //Shaders configure

    shader lightShaders("Shaders/VertexLight", "Shaders/ColorLight");

    shader cubeShaders("Shaders/VertexStand", "Shaders/fCube");
    shader backShaders("Shaders/VertexBack", "Shaders/fBack");
    shader convShaders("Shaders/VertexStand", "Shaders/fConv");
    shader prefShaders("Shaders/VertexStand", "Shaders/fPrefilter");
    shader BRDFShaders("Shaders/VertexBRDF", "Shaders/fBRDF");
    shader PBRshaders("Shaders/VertexPBR", "Shaders/fPBRspec");

    shader screenShaders("Shaders/VertexFrame","Shaders/fFrame");

    screenShaders.UseShaderProgramm();
    screenShaders.IntUniform("screenTexture", 1);

    glm::vec3 lightPos1 = glm::vec3(0.0f, 5.0f, 0.0f);
    glm::vec3 lightColor1 = glm::vec3(1.0f, 1.0f, 1.0f);
    int currLight = 1;

    glm::vec3 currPos[] =
    { glm::vec3(0.0f,  0.0f, 0.0f),
      glm::vec3(0.0f,  0.0f, 7.0f),
      glm::vec3(2.0f,  0.0f, 0.0f),
      glm::vec3(7.0f,  0.0f, 0.0f), };

    PBRshaders.UseShaderProgramm();
    PBRshaders.IntUniform("albedoMap", 1);
    PBRshaders.IntUniform("normalMap", 2);
    PBRshaders.IntUniform("metallicMap", 3);
    PBRshaders.IntUniform("roughnessMap", 4);

    PBRshaders.IntUniform("irradianceMap", 5);
    PBRshaders.IntUniform("prefilterMap", 6);
    PBRshaders.IntUniform("brdfLUT", 7);

    render.createSkybox(cubeShaders, backShaders, "PBR/night.hdr");
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


    render.createScreenFramebuffer();


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

        render.sideInit();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //glViewport(0, 0, 1920, 1080);

        //Camera and projective matrixes

        glm::mat4 view = Camera.mat4getView();
        glm::vec3 viewPos = Camera.getCameraPos();

        // Main render
        // PBR

        PBRshaders.UseShaderProgramm();
        render.IBLpbrDiff(PBRshaders);
        render.IBLpbrSpec(PBRshaders);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
        

        //PBRshaders.mat4Uniform("proj", proj);
        PBRshaders.mat4Uniform("view", view);
        PBRshaders.mat4Uniform("model", model);

        PBRshaders.vec3Uniform("lights[0].color", lightColor1);
        PBRshaders.vec3Uniform("lights[0].pos", lightPos1);
        PBRshaders.vec3Uniform("lights[0].atten", 1.0f, 0.045f, 0.0075f);
        PBRshaders.FloatUniform("lights[0].diff", 1.0f);
        PBRshaders.FloatUniform("lights[0].spec", 0.9f);

        glm::vec3 Pos = glm::vec3(currPos[0].x, currPos[0].y, currPos[0].z);
        model = glm::translate(model, Pos);
        PBRshaders.vec3Uniform("viewPos", viewPos);
        render.activePBRtextures(metalATexture);
        render.sphereRender();

        Pos = glm::vec3(currPos[0].x + 3, currPos[0].y, currPos[0].z);
        model = glm::translate(model, Pos);
        PBRshaders.mat4Uniform("model", model);
        render.activePBRtextures(marbleTexture);
        render.sphereRender();

        Pos = glm::vec3(currPos[0].x + 3, currPos[0].y, currPos[0].z);
        model = glm::translate(model, Pos);
        PBRshaders.mat4Uniform("model", model);
        render.activePBRtextures(pavingTexture);
        render.sphereRender();

        Pos = glm::vec3(currPos[0].x + 3, currPos[0].y, currPos[0].z);
        model = glm::translate(model, Pos);
        PBRshaders.mat4Uniform("model", model);
        render.activePBRtextures(metalBTexture);
        render.sphereRender();

        //Lighting render

        lightShaders.UseShaderProgramm();

        glm::mat4 lightModel1 = glm::mat4(1.0f);
        lightModel1 = glm::translate(lightModel1, lightPos1);
        lightModel1 = glm::scale(lightModel1, glm::vec3(0.05f));

        lightShaders.mat4Uniform("model", lightModel1);
        lightShaders.mat4Uniform("proj", proj);
        lightShaders.mat4Uniform("view", view);
        lightShaders.vec3Uniform("lightColor", lightColor1);

        //render.sphereRender();

        //Skybox render
        backShaders.UseShaderProgramm();
        backShaders.mat4Uniform("view", view);
        render.skyboxRender();

        render.sideRender(screenShaders);

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
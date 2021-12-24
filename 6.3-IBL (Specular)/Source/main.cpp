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
    unsigned int metalATexture1 = render.loadTexture("PBR/metalA/albedo.jpg");
    unsigned int metalATexture2 = render.loadTexture("PBR/metalA/normal.jpg");
    unsigned int metalATexture3 = render.loadTexture("PBR/metalA/metallic.jpg");
    unsigned int metalATexture4 = render.loadTexture("PBR/metalA/roughness.jpg");

    unsigned int marbleTexture1 = render.loadTexture("PBR/marble/albedo.jpg");
    unsigned int marbleTexture2 = render.loadTexture("PBR/marble/normal.jpg");
    unsigned int marbleTexture3 = render.loadTexture("PBR/marble/metallic.jpg");
    unsigned int marbleTexture4 = render.loadTexture("PBR/marble/roughness.jpg");

    unsigned int pavingTexture1 = render.loadTexture("PBR/paving/albedo.jpg");
    unsigned int pavingTexture2 = render.loadTexture("PBR/paving/normal.jpg");
    unsigned int pavingTexture3 = render.loadTexture("PBR/paving/metallic.jpg");
    unsigned int pavingTexture4 = render.loadTexture("PBR/paving/roughness.jpg");

    unsigned int metalBTexture1 = render.loadTexture("PBR/metalB/albedo.jpg");
    unsigned int metalBTexture2 = render.loadTexture("PBR/metalB/normal.jpg");
    unsigned int metalBTexture3 = render.loadTexture("PBR/metalB/metallic.jpg");
    unsigned int metalBTexture4 = render.loadTexture("PBR/metalB/roughness.jpg");

    //Shaders configure

    shader lightShaders("Shaders/VertexLight", "Shaders/ColorLight");

    shader cubeShaders("Shaders/VertexStand", "Shaders/fCube");
    shader backShaders("Shaders/VertexBack", "Shaders/fBack");
    shader convShaders("Shaders/VertexStand", "Shaders/fConv");
    shader prefShaders("Shaders/VertexStand", "Shaders/fPrefilter");
    shader BRDFShaders("Shaders/VertexBRDF", "Shaders/fBRDF");
    shader PBRshaders("Shaders/VertexPBR", "Shaders/fPBRspec");


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
        glm::vec3 Pos = glm::vec3(currPos[0].x, currPos[0].y, currPos[0].z);
        model = glm::translate(model, Pos);

        //PBRshaders.mat4Uniform("proj", proj);
        PBRshaders.mat4Uniform("view", view);
        PBRshaders.mat4Uniform("model", model);

        PBRshaders.vec3Uniform("lights[0].color", lightColor1);
        PBRshaders.vec3Uniform("lights[0].pos", lightPos1);
        PBRshaders.vec3Uniform("lights[0].atten", 1.0f, 0.045f, 0.0075f);
        PBRshaders.FloatUniform("lights[0].diff", 1.0f);
        PBRshaders.FloatUniform("lights[0].spec", 0.9f);

        PBRshaders.vec3Uniform("viewPos", viewPos);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, metalATexture1);;
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, metalATexture2);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, metalATexture3);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, metalATexture4);
        render.sphereRender();

        Pos = glm::vec3(currPos[0].x + 3, currPos[0].y, currPos[0].z);
        model = glm::translate(model, Pos);
        PBRshaders.mat4Uniform("model", model);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, marbleTexture1);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, marbleTexture2);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, marbleTexture3);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, marbleTexture4);
        render.sphereRender();

        Pos = glm::vec3(currPos[0].x + 3, currPos[0].y, currPos[0].z);
        model = glm::translate(model, Pos);
        PBRshaders.mat4Uniform("model", model);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, pavingTexture1);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, pavingTexture2);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, pavingTexture3);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, pavingTexture4);
        render.sphereRender();

        Pos = glm::vec3(currPos[0].x + 3, currPos[0].y, currPos[0].z);
        model = glm::translate(model, Pos);
        PBRshaders.mat4Uniform("model", model);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, metalBTexture1);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, metalBTexture2);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, metalBTexture3);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, metalBTexture4);
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
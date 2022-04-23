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
    //glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    window = glfwCreateWindow(1920, 1080, "OpenGL", NULL, NULL);
    glfwMakeContextCurrent(window);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);



    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    objectsRender render;

    std::vector<unsigned> goldTexture = render.loadPBRtextures("PBR/gold");
    std::vector<unsigned> marbleTexture = render.loadPBRtextures("PBR/marble");
    std::vector<unsigned> mirrorTexture = render.loadPBRtextures("PBR/metalG");
    std::vector<unsigned> ceramicTexture = render.loadPBRtextures("PBR/ñeramicG");

    //Shaders configure

    shader lightShaders("Shaders/VertexLight", "Shaders/ColorLight");

    shader cubeShaders("Shaders/VertexStand", "Shaders/fCube");
    shader backShaders("Shaders/VertexBack", "Shaders/fBack");
    shader convShaders("Shaders/VertexStand", "Shaders/fConv");
    shader prefShaders("Shaders/VertexStand", "Shaders/fPrefilter");
    shader BRDFShaders("Shaders/VertexBRDF", "Shaders/fBRDF");
    shader PBRshaders("Shaders/VertexPBR", "Shaders/fPBRspec");

    shader screenShaders("Shaders/VertexFrame","Shaders/fFrame");
    shader backEnvShaders("Shaders/VertexBack", "Shaders/fEnv");
    shader convEnvShaders("Shaders/VertexStand", "Shaders/fConv");
    shader prefEnvShaders("Shaders/VertexStand", "Shaders/fPrefilter");
    shader BRDFEnvShaders("Shaders/VertexBRDF", "Shaders/fBRDF");
    shader PBREnvshaders("Shaders/VertexPBR", "Shaders/fPBRspecEnv");

    screenShaders.UseShaderProgramm();
    screenShaders.IntUniform("screenTexture", 1);

    glm::vec3 lightPos1 = glm::vec3(0.0f, 5.0f, 0.0f);
    glm::vec3 lightColor1 = glm::vec3(1.0f, 1.0f, 1.0f);
    int currLight = 1;

    glm::vec3 currPos[] =
    { glm::vec3(0.0f,  0.0f, 0.0f),
      glm::vec3(4.0f,  0.0f, 0.0f), 
      glm::vec3(0.0f,  0.0f, 4.0f),
      glm::vec3(4.0f,  0.0f, 4.0f), };

    PBRshaders.UseShaderProgramm();
    PBRshaders.IntUniform("albedoMap", 1);
    PBRshaders.IntUniform("normalMap", 2);
    PBRshaders.IntUniform("metallicMap", 3);
    PBRshaders.IntUniform("roughnessMap", 4);

    PBRshaders.IntUniform("irradianceMap", 5);
    PBRshaders.IntUniform("prefilterMap", 6);
    PBRshaders.IntUniform("brdfLUT", 7);

    PBREnvshaders.UseShaderProgramm();
    PBREnvshaders.IntUniform("albedoMap", 1);
    PBREnvshaders.IntUniform("normalMap", 2);
    PBREnvshaders.IntUniform("metallicMap", 3);
    PBREnvshaders.IntUniform("roughnessMap", 4);

    PBREnvshaders.IntUniform("irradianceMap", 5);
    PBREnvshaders.IntUniform("prefilterMap", 6);
    PBREnvshaders.IntUniform("brdfLUT", 7);

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
            //First sphere
            render.createEnvFramebuffer();
            render.createEnvMap(0);
            render.setCaptureViews(currPos[0]);
            viewPos = currPos[0];
            for (unsigned i = 0; i < 6; ++i)
            {
                render.sideInit();
                render.saveEnvMapSide(screenShaders, i, 0);

                glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                PBREnvshaders.UseShaderProgramm();

                glm::mat4 model = glm::mat4(1.0f);
                view = render.getCaptureView(i);
                proj = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);

                PBREnvshaders.mat4Uniform("proj", proj);
                PBREnvshaders.mat4Uniform("view", view);
                PBREnvshaders.vec3Uniform("viewPos", viewPos);

                PBREnvshaders.vec3Uniform("lights[0].color", lightColor1);
                PBREnvshaders.vec3Uniform("lights[0].pos", lightPos1);
                PBREnvshaders.vec3Uniform("lights[0].atten", 1.0f, 0.045f, 0.0075f);
                PBREnvshaders.FloatUniform("lights[0].diff", 1.0f);
                PBREnvshaders.FloatUniform("lights[0].spec", 0.9f);

                glm::vec3 Pos = glm::vec3(currPos[1]);
                model = glm::mat4(1.0f);
                model = glm::translate(model, Pos);                
                PBREnvshaders.mat4Uniform("model", model);
                render.activePBRtextures(marbleTexture);
                render.IBLactive(PBREnvshaders);
                render.sphereRender();

                Pos = glm::vec3(currPos[2]);
                model = glm::mat4(1.0f);
                model = glm::translate(model, Pos);
                PBREnvshaders.mat4Uniform("model", model);
                render.activePBRtextures(goldTexture);
                render.IBLactive(PBREnvshaders);
                render.sphereRender();

                Pos = glm::vec3(currPos[3]);
                model = glm::mat4(1.0f);
                model = glm::translate(model, Pos);
                PBREnvshaders.mat4Uniform("model", model);
                render.activePBRtextures(ceramicTexture);
                render.IBLactive(PBREnvshaders);
                render.sphereRender();

                //Skybox render                               
                backEnvShaders.UseShaderProgramm();
                backEnvShaders.mat4Uniform("view", view);
                backEnvShaders.mat4Uniform("proj", proj);
                render.envRender(backEnvShaders);
            }

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            render.createEnvDiffuse(convEnvShaders, currPos[0], 0);
            render.createEnvSpec(prefEnvShaders, currPos[0], 0);
            render.createEnvBRDF(BRDFEnvShaders, currPos[0], 0);

            //Second sphere
            render.createEnvFramebuffer();
            render.createEnvMap(1);
            render.setCaptureViews(currPos[1]);
            viewPos = currPos[1];
            for (unsigned i = 0; i < 6; ++i)
            {
                render.sideInit();
                render.saveEnvMapSide(screenShaders, i, 1);

                glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                PBREnvshaders.UseShaderProgramm();

                glm::mat4 model = glm::mat4(1.0f);
                view = render.getCaptureView(i);
                proj = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);

                PBREnvshaders.mat4Uniform("proj", proj);
                PBREnvshaders.mat4Uniform("view", view);
                PBREnvshaders.vec3Uniform("viewPos", viewPos);

                PBREnvshaders.vec3Uniform("lights[0].color", lightColor1);
                PBREnvshaders.vec3Uniform("lights[0].pos", lightPos1);
                PBREnvshaders.vec3Uniform("lights[0].atten", 1.0f, 0.045f, 0.0075f);
                PBREnvshaders.FloatUniform("lights[0].diff", 1.0f);
                PBREnvshaders.FloatUniform("lights[0].spec", 0.9f);

                glm::vec3 Pos = glm::vec3(currPos[0] - viewPos);
                model = glm::mat4(1.0f);
                model = glm::translate(model, Pos);
                PBREnvshaders.mat4Uniform("model", model);
                render.activePBRtextures(mirrorTexture);
                render.IBLactive(PBREnvshaders);
                render.sphereRender();

                Pos = glm::vec3(currPos[2] - viewPos);
                model = glm::mat4(1.0f);
                model = glm::translate(model, Pos);
                PBREnvshaders.mat4Uniform("model", model);
                render.activePBRtextures(goldTexture);
                render.IBLactive(PBREnvshaders);
                render.sphereRender();

                Pos = glm::vec3(currPos[3] - viewPos);
                model = glm::mat4(1.0f);
                model = glm::translate(model, Pos);
                PBREnvshaders.mat4Uniform("model", model);
                render.activePBRtextures(ceramicTexture);
                render.IBLactive(PBREnvshaders);
                render.sphereRender();

                //Skybox render
                backEnvShaders.UseShaderProgramm();
                backEnvShaders.mat4Uniform("view", view);
                backEnvShaders.mat4Uniform("proj", proj);
                render.envRender(backEnvShaders);
            }

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            render.createEnvDiffuse(convEnvShaders, currPos[1], 1);
            render.createEnvSpec(prefEnvShaders, currPos[1], 1);
            render.createEnvBRDF(BRDFEnvShaders, currPos[1], 1);


            //Third sphere
            render.createEnvFramebuffer();
            render.createEnvMap(2);
            render.setCaptureViews(currPos[2]);
            viewPos = currPos[2];
            for (unsigned i = 0; i < 6; ++i)
            {
                render.sideInit();
                render.saveEnvMapSide(screenShaders, i, 2);

                glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                PBREnvshaders.UseShaderProgramm();

                glm::mat4 model = glm::mat4(1.0f);
                view = render.getCaptureView(i);
                proj = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);

                PBREnvshaders.mat4Uniform("proj", proj);
                PBREnvshaders.mat4Uniform("view", view);
                PBREnvshaders.vec3Uniform("viewPos", viewPos);

                PBREnvshaders.vec3Uniform("lights[0].color", lightColor1);
                PBREnvshaders.vec3Uniform("lights[0].pos", lightPos1);
                PBREnvshaders.vec3Uniform("lights[0].atten", 1.0f, 0.045f, 0.0075f);
                PBREnvshaders.FloatUniform("lights[0].diff", 1.0f);
                PBREnvshaders.FloatUniform("lights[0].spec", 0.9f);

                glm::vec3 Pos = glm::vec3(currPos[0] - viewPos);
                model = glm::mat4(1.0f);
                model = glm::translate(model, Pos);
                PBREnvshaders.mat4Uniform("model", model);
                render.activePBRtextures(mirrorTexture);
                render.IBLactive(PBREnvshaders);
                render.sphereRender();

                Pos = glm::vec3(currPos[1] - viewPos);
                model = glm::mat4(1.0f);
                model = glm::translate(model, Pos);
                PBREnvshaders.mat4Uniform("model", model);
                render.activePBRtextures(marbleTexture);
                render.IBLactive(PBREnvshaders);
                render.sphereRender();

                Pos = glm::vec3(currPos[3] - viewPos);
                model = glm::mat4(1.0f);
                model = glm::translate(model, Pos);
                PBREnvshaders.mat4Uniform("model", model);
                render.activePBRtextures(ceramicTexture);
                render.IBLactive(PBREnvshaders);
                render.sphereRender();

                //Skybox render
                backEnvShaders.UseShaderProgramm();
                backEnvShaders.mat4Uniform("view", view);
                backEnvShaders.mat4Uniform("proj", proj);
                render.envRender(backEnvShaders);
            }

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            render.createEnvDiffuse(convEnvShaders, currPos[2], 2);
            render.createEnvSpec(prefEnvShaders, currPos[2], 2);
            render.createEnvBRDF(BRDFEnvShaders, currPos[2], 2);

            //Fourth sphere
            render.createEnvFramebuffer();
            render.createEnvMap(3);
            render.setCaptureViews(currPos[3]);
            viewPos = currPos[3];
            for (unsigned i = 0; i < 6; ++i)
            {
                render.sideInit();
                render.saveEnvMapSide(screenShaders, i, 3);

                glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                PBREnvshaders.UseShaderProgramm();

                glm::mat4 model = glm::mat4(1.0f);
                view = render.getCaptureView(i);
                proj = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);

                PBREnvshaders.mat4Uniform("proj", proj);
                PBREnvshaders.mat4Uniform("view", view);
                PBREnvshaders.vec3Uniform("viewPos", viewPos);

                PBREnvshaders.vec3Uniform("lights[0].color", lightColor1);
                PBREnvshaders.vec3Uniform("lights[0].pos", lightPos1);
                PBREnvshaders.vec3Uniform("lights[0].atten", 1.0f, 0.045f, 0.0075f);
                PBREnvshaders.FloatUniform("lights[0].diff", 1.0f);
                PBREnvshaders.FloatUniform("lights[0].spec", 0.9f);

                glm::vec3 Pos = glm::vec3(currPos[0] - viewPos);
                model = glm::mat4(1.0f);
                model = glm::translate(model, Pos);
                PBREnvshaders.mat4Uniform("model", model);
                render.activePBRtextures(mirrorTexture);
                render.IBLactive(PBREnvshaders);
                render.sphereRender();

                Pos = glm::vec3(currPos[1] - viewPos);
                model = glm::mat4(1.0f);
                model = glm::translate(model, Pos);
                PBREnvshaders.mat4Uniform("model", model);
                render.activePBRtextures(marbleTexture);
                render.IBLactive(PBREnvshaders);
                render.sphereRender();

                Pos = glm::vec3(currPos[2] - viewPos);
                model = glm::mat4(1.0f);
                model = glm::translate(model, Pos);
                PBREnvshaders.mat4Uniform("model", model);
                render.activePBRtextures(goldTexture);
                render.IBLactive(PBREnvshaders);
                render.sphereRender();

                //Skybox render
                backEnvShaders.UseShaderProgramm();
                backEnvShaders.mat4Uniform("view", view);
                backEnvShaders.mat4Uniform("proj", proj);
                render.envRender(backEnvShaders);
            }

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            render.createEnvDiffuse(convEnvShaders, currPos[3], 3);
            render.createEnvSpec(prefEnvShaders, currPos[3], 3);
            render.createEnvBRDF(BRDFEnvShaders, currPos[3], 3);

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

        PBRshaders.UseShaderProgramm();

        glm::mat4 model = glm::mat4(1.0f);
        proj = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);

        PBRshaders.mat4Uniform("proj", proj);
        PBRshaders.mat4Uniform("view", view);
        PBRshaders.vec3Uniform("viewPos", viewPos);

        PBRshaders.vec3Uniform("lights[0].color", lightColor1);
        PBRshaders.vec3Uniform("lights[0].pos", lightPos1);
        PBRshaders.vec3Uniform("lights[0].atten", 1.0f, 0.045f, 0.0075f);
        PBRshaders.FloatUniform("lights[0].diff", 1.0f);
        PBRshaders.FloatUniform("lights[0].spec", 0.9f);

        glm::vec3 Pos = glm::vec3(currPos[0]);
        model = glm::mat4(1.0f);
        model = glm::translate(model, Pos);
        PBRshaders.mat4Uniform("model", model);
        render.activePBRtextures(mirrorTexture);
        render.IBLpbrEnv(PBRshaders, 0);
        //render.IBLactive(PBRshaders);
        render.sphereRender();

        Pos = glm::vec3(currPos[1]);
        model = glm::mat4(1.0f);
        model = glm::translate(model, Pos);
        PBRshaders.mat4Uniform("model", model);
        render.activePBRtextures(marbleTexture);
        render.IBLpbrEnv(PBRshaders, 1);
        //render.IBLactive(PBRshaders);
        render.sphereRender();

        Pos = glm::vec3(currPos[2]); 
        model = glm::mat4(1.0f);
        model = glm::translate(model, Pos); 
        PBRshaders.mat4Uniform("model", model);
        render.activePBRtextures(goldTexture);
        render.IBLpbrEnv(PBRshaders, 2);
        //render.IBLactive(PBRshaders);
        render.sphereRender();

        Pos = glm::vec3(currPos[3]);
        model = glm::mat4(1.0f);
        model = glm::translate(model, Pos);
        PBRshaders.mat4Uniform("model", model);
        render.activePBRtextures(ceramicTexture);
        render.IBLpbrEnv(PBRshaders, 3);
        //render.IBLactive(PBRshaders);
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
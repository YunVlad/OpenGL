#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "shader.h"
#include "camera.h"
#include "objectsRender.h"
#include "model.h"


camera Camera(glm::vec3(2.0f, 3.5f, 8.5f), glm::vec3(0.0f, 1.0f, 0.0f), 270.0f, -32.0f);
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

    model wind("Data/Models/window.stl");
    model wall("Data/Models/wall.stl");
    model wallN("Data/Models/wallN.stl");

    std::vector <model> modelsPack;
    modelsPack.push_back(wind);
    modelsPack.push_back(wallN);
    modelsPack.push_back(wall);

    std::vector<unsigned> testTexture = render.loadPBRtextures("Data/Textures/test05N");
    std::vector<unsigned> testMetalTexture = render.loadPBRtextures("Data/Textures/test0M");

    std::vector<unsigned> woodTexture = render.loadPBRtextures("Data/Textures/PBRwoodN");
    std::vector<unsigned> goldTexture = render.loadPBRtextures("Data/Textures/PBRgoldM");
    std::vector<unsigned> paveTexture = render.loadPBRtextures("Data/Textures/PBRpaveN");
    std::vector<unsigned> pipeTexture = render.loadPBRtextures("Data/Textures/PBRpipeM");
    std::vector<unsigned> tileTexture = render.loadPBRtextures("Data/Textures/PBRtileN");

    std::vector<std::vector<unsigned>> texturePack;
    texturePack.push_back(tileTexture);
    texturePack.push_back(goldTexture);
    texturePack.push_back(woodTexture);
    texturePack.push_back(pipeTexture);
    texturePack.push_back(paveTexture);

    //Shaders configure
    shader lightShaders("Data/Shaders/Light.vert", "Data/Shaders/Light.frag");
    shader shadowShaders("Data/Shaders/Shadow.vert", "Data/Shaders/Shadow.frag", "Data/Shaders/Shadow.geom");
    shader PBRShaders("Data/Shaders/PBRShadows.vert", "Data/Shaders/PBRShadows.frag");
    shader PBRNTShaders("Data/Shaders/PBRNoTexturesShadows.vert", "Data/Shaders/PBRNoTexturesShadows.frag");

    shader IBLShaders("Data/Shaders/IBLLights.vert", "Data/Shaders/IBLLights.frag");
    shader IBLNTShaders("Data/Shaders/IBLNoTextures.vert", "Data/Shaders/IBLNoTextures.frag");

    shader IBLEnvShaders("Data/ShadersIBL/IBLforEnv.vert", "Data/ShadersIBL/IBLforEnv.frag");
    shader backEnvShaders("Data/ShadersIBL/BackEnv.vert", "Data/ShadersIBL/BackEnv.frag");

    shader cubeShaders("Data/ShadersIBL/Cube.vert", "Data/ShadersIBL/Cube.frag");
    shader backShaders("Data/ShadersIBL/Back.vert", "Data/ShadersIBL/Back.frag");
    shader convShaders("Data/ShadersIBL/Convolution.vert", "Data/ShadersIBL/Convolution.frag");
    shader prefShaders("Data/ShadersIBL/Prefilter.vert", "Data/ShadersIBL/Prefilter.frag");
    shader BRDFShaders("Data/ShadersIBL/BRDF.vert", "Data/ShadersIBL/BRDF.frag");
    
    render.bindPBRmaps(PBRShaders);
    render.shadowMapBind(PBRShaders);
    render.shadowMapBind(PBRNTShaders);

    render.bindPBRmaps(IBLShaders);
    render.bindIBLmaps(IBLShaders);

    render.bindPBRmaps(IBLNTShaders);
    render.bindIBLmaps(IBLNTShaders);

    render.bindPBRmaps(IBLEnvShaders);
    render.bindIBLmaps(IBLEnvShaders);

    render.initBuffers();

    glm::mat4 proj = glm::mat4(1.0f);
    proj = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);
    IBLShaders.UseShaderProgramm();
    IBLShaders.mat4Uniform("proj", proj);

    IBLEnvShaders.UseShaderProgramm();
    IBLEnvShaders.mat4Uniform("proj", proj);

    backShaders.UseShaderProgramm();
    backShaders.mat4Uniform("proj", proj);

    glm::vec3 lightPos1 = glm::vec3(2.0f, 4.0f, 2.0f);
    glm::vec3 lightColor1 = glm::vec3(1.0f, 1.0f, 1.0f);
    int currLight = 1;

    std::vector<glm::vec3> currPos =
    { glm::vec3(0.0f,  0.0f, 0.0f),
      glm::vec3(2.0f,  0.0f, 2.0f), 
      glm::vec3(4.0f,  0.0f, 0.0f),
      glm::vec3(0.0f,  0.0f, 4.0f),
      glm::vec3(4.0f,  0.0f, 4.0f), };

    std::vector<glm::vec3> wallPos =
    { glm::vec3(-2.0f, -2.0f,  0.0f), 
      glm::vec3( 6.0f, -2.0f,  0.0f), 
      glm::vec3( 2.0f, -2.0f, -4.0f), };

    glm::vec3 platePos = glm::vec3(0.0f, -2.0f, 0.0f);


    int scrWidth, scrHeight;
    glfwGetFramebufferSize(window, &scrWidth, &scrHeight);
    glViewport(0, 0, scrWidth, scrHeight);

    render.createMapSet(5);
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

        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            preRender = true;

        glm::mat4 view;
        glm::vec3 viewPos;

        if (preRender)
        {
            render.shadowMapCreate(lightPos1, shadowShaders);

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, platePos);
            model = glm::scale(model, glm::vec3(50.0f));
            shadowShaders.mat4Uniform("model", model);
            render.plateRender();

            render.objectPBRAllmodels(shadowShaders, wallPos, modelsPack);

            for (unsigned i = 0; i < 5; ++i)
            {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, currPos[i]);
                model = glm::scale(model, glm::vec3(0.8f));
                shadowShaders.mat4Uniform("model", model);
                render.sphereRender();
            }

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glViewport(0, 0, scrWidth, scrHeight);

            PBRShaders.UseShaderProgramm();
            PBRShaders.vec3Uniform("lights[0].color", lightColor1);
            PBRShaders.vec3Uniform("lights[0].pos", lightPos1);
            PBRShaders.vec3Uniform("lights[0].atten", 1.0f, 0.045f, 0.0075f);
            PBRShaders.FloatUniform("lights[0].diff", 1.0f);
            PBRShaders.FloatUniform("lights[0].spec", 0.9f);

            PBRNTShaders.UseShaderProgramm();
            PBRNTShaders.vec3Uniform("lights[0].color", lightColor1);
            PBRNTShaders.vec3Uniform("lights[0].pos", lightPos1);
            PBRNTShaders.vec3Uniform("lights[0].atten", 1.0f, 0.045f, 0.0075f);
            PBRNTShaders.FloatUniform("lights[0].diff", 1.0f);
            PBRNTShaders.FloatUniform("lights[0].spec", 0.9f);

            for (unsigned nCurr = 0; nCurr < 5; ++nCurr)
            {
                render.createEnvFramebuffer();
                render.createEnvMap(nCurr);
                render.setCaptureViews(currPos[nCurr]);
                glm::mat4 view;
                glm::mat4 proj = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
                glm::vec3 viewPos = currPos[nCurr];

                for (unsigned i = 0; i < 6; ++i)
                {
                    render.sideInit();
                    render.saveEnvMapSide(i, nCurr);

                    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                    PBRShaders.UseShaderProgramm();

                    glm::mat4 model = glm::mat4(1.0f);
                    view = render.getCaptureView(i);

                    PBRShaders.mat4Uniform("view", view);
                    PBRShaders.mat4Uniform("proj", proj);
                    PBRShaders.vec3Uniform("viewPos", viewPos);

                    unsigned curr;
                    for (unsigned i = 1; i < 5; ++i)
                    {
                        curr = (i + nCurr) % 5;
                        render.objectPBRsphere(PBRShaders, currPos[curr] - viewPos, texturePack[curr]);
                    }

                    PBRNTShaders.UseShaderProgramm();
                    PBRNTShaders.mat4Uniform("proj", proj);
                    PBRNTShaders.mat4Uniform("view", view);
                    PBRNTShaders.vec3Uniform("viewPos", viewPos);

                    PBRNTShaders.vec3Uniform("albedo", glm::vec3(0.8f, 0.8f, 0.8f));
                    PBRNTShaders.FloatUniform("metallic", 1.0f);
                    PBRNTShaders.FloatUniform("roughness", 0.5f);
                    render.shadowMapActive(PBRNTShaders);

                    model = glm::mat4(1.0f);
                    model = glm::translate(model, platePos - viewPos);
                    model = glm::scale(model, glm::vec3(50.0f));
                    PBRNTShaders.mat4Uniform("model", model);
                    render.plateRender();

                    render.objectPBRAllmodels(PBRNTShaders, wallPos, modelsPack);

                    lightShaders.UseShaderProgramm();
                    model = glm::mat4(1.0f);
                    model = glm::translate(model, lightPos1);
                    model = glm::scale(model, glm::vec3(0.05f));
                    lightShaders.mat4Uniform("model", model);
                    lightShaders.mat4Uniform("proj", proj);
                    lightShaders.mat4Uniform("view", view);
                    lightShaders.vec3Uniform("lightColor", lightColor1);
                    render.sphereRender();
                }

                render.createAllEnvMaps(convShaders, prefShaders, BRDFShaders, currPos[nCurr], nCurr);
            }

            preRender = false;
        }


        // Main render
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, scrWidth, scrHeight);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);


        IBLShaders.UseShaderProgramm();
        glm::mat4 proj = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);
        view = Camera.mat4getView();
        viewPos = Camera.getCameraPos();
        IBLShaders.mat4Uniform("proj", proj);
        IBLShaders.mat4Uniform("view", view);
        IBLShaders.vec3Uniform("viewPos", viewPos);

        IBLShaders.vec3Uniform("lights[0].color", lightColor1);
        IBLShaders.vec3Uniform("lights[0].pos", lightPos1);
        IBLShaders.vec3Uniform("lights[0].atten", 1.0f, 0.045f, 0.0075f);
        IBLShaders.FloatUniform("lights[0].diff", 1.0f);
        IBLShaders.FloatUniform("lights[0].spec", 0.9f);

        for (unsigned i = 0; i < 5; ++i)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, currPos[i]);
            model = glm::scale(model, glm::vec3(0.8f));
            IBLShaders.mat4Uniform("model", model);
            render.activePBRtextures(texturePack[i]);
            render.IBLEnvactive(IBLShaders, i);
            render.sphereRender();
        }


        PBRNTShaders.UseShaderProgramm();
        PBRNTShaders.mat4Uniform("proj", proj);
        PBRNTShaders.mat4Uniform("view", view);
        PBRNTShaders.vec3Uniform("viewPos", viewPos);

        PBRNTShaders.vec3Uniform("lights[0].color", lightColor1);
        PBRNTShaders.vec3Uniform("lights[0].pos", lightPos1);
        PBRNTShaders.vec3Uniform("lights[0].atten", 1.0f, 0.045f, 0.0075f);
        PBRNTShaders.FloatUniform("lights[0].diff", 1.0f);
        PBRNTShaders.FloatUniform("lights[0].spec", 0.9f);

        PBRNTShaders.vec3Uniform("albedo", glm::vec3(0.8f, 0.8f, 0.8f));
        PBRNTShaders.FloatUniform("metallic", 1.0f);
        PBRNTShaders.FloatUniform("roughness", 0.5f);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, platePos);
        model = glm::scale(model, glm::vec3(50.0f));
        PBRNTShaders.mat4Uniform("model", model);
        render.shadowMapActive(PBRNTShaders);
        render.plateRender();

        PBRNTShaders.vec3Uniform("albedo", glm::vec3(0.3f, 0.3f, 0.3f));
        PBRNTShaders.FloatUniform("metallic", 1.0f);
        PBRNTShaders.FloatUniform("roughness", 0.5f);
        render.shadowMapActive(PBRNTShaders);

        render.objectPBRAllmodels(PBRNTShaders, wallPos, modelsPack);


        lightShaders.UseShaderProgramm();
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos1);
        model = glm::scale(model, glm::vec3(0.05f));
        lightShaders.mat4Uniform("model", model);
        lightShaders.mat4Uniform("proj", proj);
        lightShaders.mat4Uniform("view", view);
        lightShaders.vec3Uniform("lightColor", lightColor1);
        render.sphereRender();

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
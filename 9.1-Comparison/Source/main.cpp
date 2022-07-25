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

    std::vector<unsigned> testTexture = render.loadPBRtextures("Data/Textures/test05N");
    std::vector<unsigned> testMetalTexture = render.loadPBRtextures("Data/Textures/test0M");

    std::vector<unsigned> sandTexture = render.loadPBRtextures("Data/Textures/PBRsandN");
    std::vector<unsigned> paveTexture = render.loadPBRtextures("Data/Textures/PBRpaveN");
    std::vector<unsigned> copperTexture = render.loadPBRtextures("Data/Textures/PBRcopperM");
    std::vector<unsigned> pipeTexture = render.loadPBRtextures("Data/Textures/PBRpipeM");
    std::vector<unsigned> tileTexture = render.loadPBRtextures("Data/Textures/PBRtileM");   

    std::vector<std::vector<unsigned>> texturePack;
    texturePack.push_back(sandTexture);
    texturePack.push_back(paveTexture);
    texturePack.push_back(copperTexture);
    texturePack.push_back(pipeTexture);
    texturePack.push_back(tileTexture);

    //Shaders configure
    shader lightShaders("Data/Shaders/Light.vert", "Data/Shaders/Light.frag");
    shader shadowShaders("Data/Shaders/Shadow.vert", "Data/Shaders/Shadow.frag", "Data/Shaders/Shadow.geom");
    shader PBRShaders("Data/Shaders/PBRShadows.vert", "Data/Shaders/PBRShadows.frag");

    shader IBLShaders("Data/Shaders/IBLLights.vert", "Data/Shaders/IBLLights.frag");

    shader cubeShaders("Data/ShadersIBL/Cube.vert", "Data/ShadersIBL/Cube.frag");
    shader backShaders("Data/ShadersIBL/Back.vert", "Data/ShadersIBL/Back.frag");
    shader convShaders("Data/ShadersIBL/Convolution.vert", "Data/ShadersIBL/Convolution.frag");
    shader prefShaders("Data/ShadersIBL/Prefilter.vert", "Data/ShadersIBL/Prefilter.frag");
    shader BRDFShaders("Data/ShadersIBL/BRDF.vert", "Data/ShadersIBL/BRDF.frag");

    shader PhongShaders("Data/Shaders/Phong.vert", "Data/Shaders/Phong.frag");
    shader PhongTexShaders("Data/Shaders/PhongTexture.vert", "Data/Shaders/PhongTexture.frag");
    
    render.bindPBRmaps(PhongTexShaders);
    render.bindPBRmaps(PBRShaders);

    render.shadowMapBind(PhongShaders);
    render.shadowMapBind(PhongTexShaders);
    render.shadowMapBind(PBRShaders);

    render.bindPBRmaps(IBLShaders);
    render.bindIBLmaps(IBLShaders);

    render.initBuffers();

    render.createSkybox(cubeShaders, backShaders, "Data/HDR/ForestOld.hdr");
    render.IBLdiffuse(convShaders);
    render.IBLspec(prefShaders);
    render.IBLbrdf(BRDFShaders);

    glm::mat4 proj = glm::mat4(1.0f);
    proj = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);
    IBLShaders.UseShaderProgramm();
    IBLShaders.mat4Uniform("proj", proj);

    backShaders.UseShaderProgramm();
    backShaders.mat4Uniform("proj", proj);

    glm::vec3 lightPos1 = glm::vec3(2.0f, 4.0f, 2.0f);
    glm::vec3 lightColor1 = glm::vec3(1.0f, 1.0f, 1.0f);
    int currLight = 1;

    std::vector<glm::vec3> currPos =
    { glm::vec3( 0.0f,  0.0f, 0.0f),
      glm::vec3( 3.0f,  0.0f, 0.0f), 
      glm::vec3( 6.0f,  0.0f, 0.0f),
      glm::vec3( 9.0f,  0.0f, 0.0f),
      glm::vec3(12.0f,  0.0f, 0.0f), 
    
      glm::vec3( 0.0f,  0.0f, 3.0f),
      glm::vec3( 3.0f,  0.0f, 3.0f),
      glm::vec3( 6.0f,  0.0f, 3.0f),
      glm::vec3( 9.0f,  0.0f, 3.0f),
      glm::vec3(12.0f,  0.0f, 3.0f),
    
      glm::vec3( 0.0f,  0.0f, 6.0f),
      glm::vec3( 3.0f,  0.0f, 6.0f),
      glm::vec3( 6.0f,  0.0f, 6.0f),
      glm::vec3( 9.0f,  0.0f, 6.0f),
      glm::vec3(12.0f,  0.0f, 6.0f), 
    
      glm::vec3( 0.0f,  0.0f, 9.0f),
      glm::vec3( 3.0f,  0.0f, 9.0f),
      glm::vec3( 6.0f,  0.0f, 9.0f),
      glm::vec3( 9.0f,  0.0f, 9.0f),
      glm::vec3(12.0f,  0.0f, 9.0f), 
    
      glm::vec3( 0.0f,  0.0f, 12.0f),
      glm::vec3( 3.0f,  0.0f, 12.0f),
      glm::vec3( 6.0f,  0.0f, 12.0f),
      glm::vec3( 9.0f,  0.0f, 12.0f),
      glm::vec3(12.0f,  0.0f, 12.0f), };

    std::vector<glm::vec3> objPos =
    { glm::vec3(0.0f,  0.0f, 0.0f), };

    glm::vec3 platePos = glm::vec3(0.0f, -2.0f, 0.0f);


    int scrWidth, scrHeight;
    glfwGetFramebufferSize(window, &scrWidth, &scrHeight);
    glViewport(0, 0, scrWidth, scrHeight);

    render.createMapSet(5);
    bool preRender = true;
    bool timeRender = true;

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

            for (int i = 0; i < 5; ++i)
                for (int j = 0; j < 3; ++j)
            {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
                glm::vec3 Pos = glm::vec3(currPos[0].x + 3 * i, currPos[0].y, currPos[0].z + 3 * j);
                model = glm::translate(model, Pos);
                shadowShaders.mat4Uniform("model", model);
                render.sphereRender();
            }

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, platePos);
            model = glm::scale(model, glm::vec3(50.0f));
            shadowShaders.mat4Uniform("model", model);
            render.plateRender();

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glViewport(0, 0, scrWidth, scrHeight);

            PBRShaders.UseShaderProgramm();
            PBRShaders.vec3Uniform("lights[0].color", lightColor1);
            PBRShaders.vec3Uniform("lights[0].pos", lightPos1);
            PBRShaders.vec3Uniform("lights[0].atten", 1.0f, 0.045f, 0.0075f);
            PBRShaders.FloatUniform("lights[0].diff", 1.0f);
            PBRShaders.FloatUniform("lights[0].spec", 0.9f);

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
                    for (unsigned j = 0; j < 5; ++j)
                    for (unsigned i = 1; i < 5; ++i)
                    {
                        curr = (i + nCurr) % 5;
                        render.objectPBRsphere(PBRShaders, currPos[curr] - viewPos, texturePack[curr]);
                    }

                    PhongShaders.UseShaderProgramm();
                    model = glm::mat4(1.0f);
                    model = glm::translate(model, platePos - viewPos);
                    model = glm::scale(model, glm::vec3(50.0f));
                    PhongShaders.mat4Uniform("proj", proj);
                    PhongShaders.mat4Uniform("view", view);
                    PhongShaders.mat4Uniform("model", model);
                    PhongShaders.vec3Uniform("material.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
                    PhongShaders.vec3Uniform("material.specular", glm::vec3(0.774597f, 0.774597f, 0.774597f));
                    PhongShaders.FloatUniform("material.shininess", 0.6f);
                    PhongShaders.vec3Uniform("lights[0].color", lightColor1);
                    PhongShaders.vec3Uniform("lights[0].pos", lightPos1);
                    PhongShaders.vec3Uniform("lights[0].atten", 1.0f, 0.09f, 0.032f);
                    PhongShaders.FloatUniform("lights[0].diff", 1.0f);
                    PhongShaders.FloatUniform("lights[0].spec", 0.9f);
                    PhongShaders.vec3Uniform("viewPos", viewPos);
                    render.shadowMapActive(PhongShaders);
                    render.plateRender();

                    lightShaders.UseShaderProgramm();
                    model = glm::mat4(1.0f);
                    model = glm::translate(model, lightPos1 - viewPos);
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

        view = Camera.mat4getView();
        viewPos = Camera.getCameraPos();
        glm::mat4 proj = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);

        IBLShaders.UseShaderProgramm();
        for (int i = 0; i < 5; ++i)
        {
            IBLShaders.mat4Uniform("proj", proj);
            IBLShaders.mat4Uniform("view", view);
            IBLShaders.vec3Uniform("viewPos", viewPos);

            IBLShaders.vec3Uniform("lights[0].color", lightColor1);
            IBLShaders.vec3Uniform("lights[0].pos", lightPos1);
            IBLShaders.vec3Uniform("lights[0].atten", 1.0f, 0.045f, 0.0075f);
            IBLShaders.FloatUniform("lights[0].diff", 1.0f);
            IBLShaders.FloatUniform("lights[0].spec", 0.9f);

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
            glm::vec3 Pos = glm::vec3(currPos[0].x + 3 * i, currPos[0].y, currPos[0].z);
            model = glm::translate(model, Pos);
            IBLShaders.mat4Uniform("model", model);
            render.activePBRtextures(texturePack[i]);
            render.IBLEnvactive(IBLShaders, i);
            render.sphereRender();
        }

        PBRShaders.UseShaderProgramm();
        for (int i = 0; i < 5; ++i)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
            glm::vec3 Pos = glm::vec3(currPos[0].x + 3 * i, currPos[0].y, currPos[0].z + 3);
            model = glm::translate(model, Pos);

            PBRShaders.mat4Uniform("proj", proj);
            PBRShaders.mat4Uniform("view", view);
            PBRShaders.mat4Uniform("model", model);

            PBRShaders.vec3Uniform("lights[0].color", lightColor1);
            PBRShaders.vec3Uniform("lights[0].pos", lightPos1);
            PBRShaders.vec3Uniform("lights[0].atten", 1.0f, 0.09f, 0.032f);
            PBRShaders.FloatUniform("lights[0].diff", 1.0f);
            PBRShaders.FloatUniform("lights[0].spec", 0.9f);

            PBRShaders.vec3Uniform("viewPos", viewPos);
            render.activePBRtextures(texturePack[i]);
            render.sphereRender();
        }

        PhongTexShaders.UseShaderProgramm();
        for (int i = 0; i < 5; ++i)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
            glm::vec3 Pos = glm::vec3(currPos[0].x + 3 * i, currPos[0].y, currPos[0].z + 6);
            model = glm::translate(model, Pos);

            PhongTexShaders.mat4Uniform("proj", proj);
            PhongTexShaders.mat4Uniform("view", view);
            PhongTexShaders.mat4Uniform("model", model);

            PhongTexShaders.vec3Uniform("lights[0].color", lightColor1);
            PhongTexShaders.vec3Uniform("lights[0].pos", lightPos1);
            PhongTexShaders.vec3Uniform("lights[0].atten", 1.0f, 0.09f, 0.032f);
            PhongTexShaders.FloatUniform("lights[0].diff", 1.0f);
            PhongTexShaders.FloatUniform("lights[0].spec", 0.9f);

            PhongTexShaders.vec3Uniform("viewPos", viewPos);
            render.activePBRtextures(texturePack[i]);
            render.sphereRender();
        }

        PhongShaders.UseShaderProgramm();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, platePos);
        model = glm::scale(model, glm::vec3(50.0f));
        PhongShaders.mat4Uniform("proj", proj);
        PhongShaders.mat4Uniform("view", view);
        PhongShaders.mat4Uniform("model", model);
        PhongShaders.vec3Uniform("material.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
        PhongShaders.vec3Uniform("material.specular", glm::vec3(0.774597f, 0.774597f, 0.774597f));
        PhongShaders.FloatUniform("material.shininess", 0.6f);
        PhongShaders.vec3Uniform("lights[0].color", lightColor1);
        PhongShaders.vec3Uniform("lights[0].pos", lightPos1);
        PhongShaders.vec3Uniform("lights[0].atten", 1.0f, 0.09f, 0.032f);
        PhongShaders.FloatUniform("lights[0].diff", 1.0f);
        PhongShaders.FloatUniform("lights[0].spec", 0.9f);
        PhongShaders.vec3Uniform("viewPos", viewPos);
        render.shadowMapActive(PhongShaders);
        render.plateRender();

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

        if (timeRender)
        {
            time = glfwGetTime();
            std::cout << time;
            timeRender = false;
        }
    }

    glfwTerminate();
    return 0;
}

int main()
{
    process();
    return 0;
}
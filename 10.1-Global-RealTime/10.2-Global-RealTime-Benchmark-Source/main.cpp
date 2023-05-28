#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "shader.h"
#include "camera.h"
#include "objectsRender.h"
//#include "model.h"


camera Camera(glm::vec3(8.6f, 4.0f, -2.5f), glm::vec3(0.0f, 1.0f, 0.0f), 145.0f, -35.0f);
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

    std::vector<unsigned> paveTexture = render.loadPBRtextures("Data/Textures/PBRpaveN");
    std::vector<unsigned> copperTexture = render.loadPBRtextures("Data/Textures/PBRcopperM");
    std::vector<unsigned> pipeTexture = render.loadPBRtextures("Data/Textures/PBRpipeM");

    std::vector<std::vector<unsigned>> texturePack;
    texturePack.push_back(paveTexture);
    texturePack.push_back(copperTexture);
    texturePack.push_back(pipeTexture);

    //Shaders configure
    shader lightShaders("Data/Shaders/Light.vert", "Data/Shaders/Light.frag");
    shader shadowShaders("Data/Shaders/Shadow.vert", "Data/Shaders/Shadow.frag", "Data/Shaders/Shadow.geom");
    shader PBRShaders("Data/Shaders/PBRShadows.vert", "Data/Shaders/PBRShadows.frag");

    shader IBLShaders("Data/Shaders/IBLLights.vert", "Data/Shaders/IBLLights.frag");

    shader cubeShaders("Data/ShadersIBL/Cube.vert", "Data/ShadersIBL/Cube.frag");
    shader backShaders("Data/ShadersIBL/Back.vert", "Data/ShadersIBL/Back.frag");
    shader convShaders("Data/ShadersIBL/Convolution.vert", "Data/ShadersIBL/Convolution.frag");
    //shader prefShaders("Data/ShadersIBL/Prefilter.vert", "Data/ShadersIBL/Prefilter.frag");
    shader prefShaders("Data/ShadersIBL/Prefilter.vert", "Data/ShadersIBL/PrefilterMod2.frag");
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

    std::vector<glm::vec3> currPos =
    { glm::vec3( 0.0f,  0.0f, 0.0f),
      glm::vec3( 3.0f,  0.0f, 3.0f), 
      glm::vec3( 6.0f,  0.0f, 6.0f),};

    std::vector<glm::vec3> objPos =
    { glm::vec3(0.0f,  0.0f, 0.0f), };

    glm::vec3 platePos = glm::vec3(0.0f, -2.0f, 0.0f);


    int scrWidth, scrHeight;
    glfwGetFramebufferSize(window, &scrWidth, &scrHeight);
    glViewport(0, 0, scrWidth, scrHeight);

    render.createMapSetMod(3);
    bool preRender = true;
    bool preRenderBefore = true;
    bool preRenderAfter = true;
    bool timeRender = true;
    int curr_obj_move = 1;

    float firstFrameTime, time, avgFrame, avgTime;
    int currFrame = 0;

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
            curr_obj_move = 1;
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
            curr_obj_move = 2;
        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
            curr_obj_move = 3;

        if ((glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) && (curr_obj_move == 1))
            lightPos1.z += 0.1f;
        if ((glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) && (curr_obj_move == 1))
            lightPos1.z -= 0.1f;
        if ((glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) && (curr_obj_move == 1))
            lightPos1.x += 0.1f;
        if ((glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) && (curr_obj_move == 1))
            lightPos1.x -= 0.1f;
        if ((glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) && (curr_obj_move == 1))
            lightPos1.y += 0.1f;
        if ((glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) && (curr_obj_move == 1))
            lightPos1.y -= 0.1f;

        if ((glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) && (curr_obj_move == 2))
            currPos[1].z += 0.1f;
        if ((glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) && (curr_obj_move == 2))
            currPos[1].z -= 0.1f;
        if ((glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) && (curr_obj_move == 2))
            currPos[1].x += 0.1f;
        if ((glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) && (curr_obj_move == 2))
            currPos[1].x -= 0.1f;
        if ((glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) && (curr_obj_move == 2))
            currPos[1].y += 0.1f;
        if ((glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) && (curr_obj_move == 2))
            currPos[1].y -= 0.1f;

        if ((glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) && (curr_obj_move == 3))
            currPos[2].z += 0.1f;
        if ((glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) && (curr_obj_move == 3))
            currPos[2].z -= 0.1f;
        if ((glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) && (curr_obj_move == 3))
            currPos[2].x += 0.1f;
        if ((glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) && (curr_obj_move == 3))
            currPos[2].x -= 0.1f;
        if ((glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) && (curr_obj_move == 3))
            currPos[2].y += 0.1f;
        if ((glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) && (curr_obj_move == 3))
            currPos[2].y -= 0.1f;

        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
            lightColor1 = glm::vec3(1.0f, 0.1f, 0.1f);
        if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
            lightColor1 = glm::vec3(0.0f, 1.0f, 0.0f);
        if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
            lightColor1 = glm::vec3(0.0f, 0.0f, 1.0f);
        if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
            lightColor1 = glm::vec3(1.0f, 1.0f, 1.0f);

        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        {
            preRender = true;
        }

        glm::mat4 view;
        glm::vec3 viewPos;

        currPos[1].x = 6 + 6*sin(time);
        currPos[1].z = 6 + 6*cos(time);
        currPos[0].x = 6*sin(time);

        //All Maps Creating
        if (preRenderBefore)
        {
            render.shadowMapGen();
            render.createEnvBRDFMod(BRDFShaders);
            render.createEnvFramebuffer();
            for (unsigned nCurr = 0; nCurr < 3; ++nCurr)
            {
                render.createEnvMapMod(nCurr);
                render.createAllEnvMapsMod2(prefShaders, currPos[nCurr], nCurr);
            }
            preRenderBefore = false;
        }

        //Shadow Process
        render.shadowMapProcess(lightPos1, shadowShaders);
        for (int i = 0; i < 3; ++i)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
            glm::vec3 Pos = glm::vec3(currPos[i].x, currPos[i].y, currPos[i].z);
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

        //Spec Map Proccess
        if (preRender)
        {
            PBRShaders.UseShaderProgramm();
            PBRShaders.vec3Uniform("lights[0].color", lightColor1);
            PBRShaders.vec3Uniform("lights[0].pos", lightPos1);
            PBRShaders.vec3Uniform("lights[0].atten", 1.0f, 0.045f, 0.0075f);
            PBRShaders.FloatUniform("lights[0].diff", 1.0f);
            PBRShaders.FloatUniform("lights[0].spec", 0.9f);

            for (unsigned nCurr = 0; nCurr < 3; ++nCurr)
            {
                render.setCaptureViews(currPos[nCurr]);
                glm::mat4 view;
                glm::mat4 proj = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
                glm::vec3 viewPos = currPos[nCurr];

                for (unsigned i = 0; i < 6; ++i)
                {
                    render.sideInit();
                    render.saveEnvMapSideMod(i, nCurr);

                    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                    PBRShaders.UseShaderProgramm();

                    glm::mat4 model = glm::mat4(1.0f);
                    view = render.getCaptureView(i);

                    PBRShaders.mat4Uniform("view", view);
                    PBRShaders.mat4Uniform("proj", proj);
                    PBRShaders.vec3Uniform("viewPos", viewPos);

                    unsigned curr, currP;
                    for (unsigned i = 0; i < 3; ++i)
                    {
                        if (i != nCurr)
                            render.objectPBRsphere(PBRShaders, currPos[i] - viewPos, texturePack[i]);
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
                render.createEnvSpecMod(prefShaders, currPos[nCurr], nCurr);
            }
            //preRender = false;
        }

        //Diff Map Creating
        if (preRenderAfter)
        {
            render.createEnvDiffuseMod2(convShaders);
            preRenderAfter = false;
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
        for (int i = 0; i < 3; ++i)
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
            glm::vec3 Pos = glm::vec3(currPos[i].x, currPos[i].y, currPos[i].z);
            model = glm::translate(model, Pos);
            IBLShaders.mat4Uniform("model", model);
            render.activePBRtextures(texturePack[i]);
            render.IBLEnvactiveMod2(IBLShaders, i);
            render.sphereRender();
        }

        PhongShaders.UseShaderProgramm();
        model = glm::mat4(1.0f);
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

        //backShaders.UseShaderProgramm();
        //backShaders.mat4Uniform("view", view);
        //backShaders.mat4Uniform("proj", proj);
        //render.testCubeMapRender(0);

        glfwSwapBuffers(window);
        glfwPollEvents();


        if (timeRender)
        {
            firstFrameTime = glfwGetTime();
            std::cout << "First frame render time: ";
            std::cout << firstFrameTime << std::endl;
            timeRender = false;
        }
        currFrame++;

        if (currFrame == 60)
        {
            avgTime = glfwGetTime();
        }

        if (currFrame == 119)
        {
            avgFrame = 60.0 / (glfwGetTime() - avgTime);
        }

        if (currFrame == 1200)
        {
            std::cout << "Benchmark time: " << glfwGetTime() - firstFrameTime << std::endl;
            std::cout << "AVG fps: " << avgFrame << std::endl;
            std::cout << "AVG of one frame time: " << avgFrame/60.0 << std::endl;
            glfwTerminate();
            return 0;
        }
    }

    glfwTerminate();
    return 0;
}

int main()
{
    process();
    system("pause");
    return 0;
}
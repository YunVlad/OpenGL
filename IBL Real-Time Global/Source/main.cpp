#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "shader.h"
#include "camera.h"
#include "objectsRender.h"
#include "rendered_structures.h"
#include "rendered_object.h"
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

float timeSin(float time)
{
    float S = sin(time / 4.0);
    if (S > 0)
        return S;
    else
        return 0.0;
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


    //Shaders configure
    shader lightShaders("Data/Shaders/Light.vert", "Data/Shaders/Light.frag");
    shader shadowShaders("Data/Shaders/Shadow.vert", "Data/Shaders/Shadow.frag", "Data/Shaders/Shadow.geom");
    //shader PBRShaders("Data/Shaders/PBRShadows.vert", "Data/Shaders/PBRShadows.frag");
    //shader PBRShaders("Data/ShadersGlobal/PBRnoShadow.vert", "Data/ShadersGlobal/PBRnoShadow.frag");
    shader PBRShaders("Data/ShadersGlobal/PBRml.vert", "Data/ShadersGlobal/PBRml.frag");
    shader ONShaders("Data/ShadersGlobal/Oren-Nayar.vert", "Data/ShadersGlobal/Oren-Nayar.frag");
    //shader IBLShaders("Data/Shaders/IBLLights.vert", "Data/Shaders/IBLLights.frag");
    shader IBLShaders("Data/ShadersGlobal/IBL.vert", "Data/ShadersGlobal/IBL.frag");
    //shader IBLShaders("Data/ShadersGlobal/IBL_Blinn.vert", "Data/ShadersGlobal/IBL_Blinn.frag");

    shader cubeShaders("Data/ShadersIBL/Cube.vert", "Data/ShadersIBL/Cube.frag");
    shader backShaders("Data/ShadersIBL/Back.vert", "Data/ShadersIBL/Back.frag");
    //shader convShaders("Data/ShadersIBL/Convolution.vert", "Data/ShadersIBL/Convolution.frag");
    shader convShaders("Data/ShadersIBL/Convolution.vert", "Data/ShadersIBL/ConvolutionMod.frag");
    //shader prefShaders("Data/ShadersIBL/Prefilter.vert", "Data/ShadersIBL/Prefilter.frag");
    shader prefShaders("Data/ShadersIBL/Prefilter.vert", "Data/ShadersIBL/PrefilterMod2.frag");
    //shader prefShaders("Data/ShadersIBL/Prefilter.vert", "Data/ShadersIBL/PrefilterMod_BlinnDistr.frag");
    shader BRDFShaders("Data/ShadersIBL/BRDF.vert", "Data/ShadersIBL/BRDF.frag");


    render.bindPBRmaps(PBRShaders);
    render.shadowMapBind(PBRShaders);

    render.bindPBRmaps(ONShaders);
    render.shadowMapBind(ONShaders);

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

    //glm::vec3 lightPos1 = glm::vec3(2.0f, 4.0f, 4.0f);
    //glm::vec3 lightPos2 = glm::vec3(12.0f, 4.0f, 12.0f);
    //glm::vec3 lightPos3 = glm::vec3(20.0f, 4.0f, -4.0f);
    glm::vec3 lightPos1 = glm::vec3(1.0f, 5.0f, 1.0f);
    glm::vec3 lightPos2 = glm::vec3(3.0f, 4.0f, 12.0f);
    glm::vec3 lightPos3 = glm::vec3(5.0f, 4.0f, -1.0f);
    glm::vec3 lightColor1 = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 lightColor2 = glm::vec3(1.0f, 0.98f, 0.09f);
    glm::vec3 lightColor3 = glm::vec3(0.09f, 0.98f, 0.09f);

    /*std::vector<glm::vec3> currPos =
    { glm::vec3(0.0f, 1.0f, 4.0f),
      glm::vec3( 5.0f,  4.0f, 4.0f),
      glm::vec3( 25.0f,  3.0f, 4.0f),};*/

    std::vector<glm::vec3> currPos =
    { glm::vec3(4.0f, 1.0f, 4.0f),
      glm::vec3(2.0f,  4.0f, 4.0f),
      glm::vec3(6.0f,  3.0f, 4.0f), };

    std::vector<glm::vec3> columnPos =
    { glm::vec3(4.0f,  0.0f, 0.0f),
      glm::vec3(8.0f,  0.0f, 0.0f),
      glm::vec3(12.0f,  0.0f, 0.0f),
      glm::vec3(16.0f,  0.0f, 0.0f),
      glm::vec3(20.0f,  0.0f, 0.0f),

      glm::vec3(4.0f,  0.0f, 8.0f),
      glm::vec3(8.0f,  0.0f, 8.0f),
      glm::vec3(12.0f,  0.0f, 8.0f),
      glm::vec3(16.0f,  0.0f, 8.0f),
      glm::vec3(20.0f,  0.0f, 8.0f), };


    glm::vec3 platePos = glm::vec3(0.0f, -2.0f, 0.0f);

    light_source source1 = { lightPos1, lightColor1, glm::vec3(1.0f, 0.014f, 0.0007f), 1.0f, 0.9f };
    light_source source2 = { lightPos2, lightColor2, glm::vec3(1.0f, 0.09f, 0.032f), 1.0f, 0.9f };
    light_source source3 = { lightPos3, lightColor3, glm::vec3(1.0f, 0.09f, 0.032f), 1.0f, 0.9f };

    rendered_object plate;
    plate.bind_shader(&PBRShaders);
    plate.load_all_textures("Data/Textures/PBRventilationM");

    rendered_object universal_prerender;
    universal_prerender.bind_shader(&ONShaders);

    rendered_object universal_render;
    universal_render.bind_shader(&IBLShaders);

    rendered_object light_render;
    light_render.bind_shader(&lightShaders);

    std::vector<rendered_object> spheres;
    for (int i = 0; i < 3; ++i)
    {
        spheres.push_back(rendered_object());      
    }
    spheres[0].load_all_textures("Data/Textures/PBRtitaniumM");
    spheres[0].bind_shader(&IBLShaders);
    spheres[1].load_all_textures("Data/Textures/PBRpipeM");
    spheres[1].bind_shader(&ONShaders);
    spheres[2].load_all_textures("Data/Textures/PBRstones0N");
    spheres[2].bind_shader(&ONShaders);

    rendered_object columns;
    columns.bind_shader(&ONShaders);
    columns.load_all_textures("Data/Textures/PBRtileM");

    int scrWidth, scrHeight;
    glfwGetFramebufferSize(window, &scrWidth, &scrHeight);
    glViewport(0, 0, scrWidth, scrHeight);

    render.createMapSetMod(1);
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
            source1.pos.z += 0.1f;
        if ((glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) && (curr_obj_move == 1))
            source1.pos.z -= 0.1f;
        if ((glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) && (curr_obj_move == 1))
            source1.pos.x += 0.1f;
        if ((glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) && (curr_obj_move == 1))
            source1.pos.x -= 0.1f;
        if ((glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) && (curr_obj_move == 1))
            source1.pos.y += 0.1f;
        if ((glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) && (curr_obj_move == 1))
            source1.pos.y -= 0.1f;

        if ((glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) && (curr_obj_move == 2))
            source2.pos.z += 0.1f;
        if ((glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) && (curr_obj_move == 2))
            source2.pos.z -= 0.1f;
        if ((glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) && (curr_obj_move == 2))
            source2.pos.x += 0.1f;
        if ((glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) && (curr_obj_move == 2))
            source2.pos.x -= 0.1f;
        if ((glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) && (curr_obj_move == 2))
            source2.pos.y += 0.1f;
        if ((glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) && (curr_obj_move == 2))
            source2.pos.y -= 0.1f;

        if ((glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) && (curr_obj_move == 3))
            source3.pos.z += 0.1f;
        if ((glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) && (curr_obj_move == 3))
            source3.pos.z -= 0.1f;
        if ((glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) && (curr_obj_move == 3))
            source3.pos.x += 0.1f;
        if ((glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) && (curr_obj_move == 3))
            source3.pos.x -= 0.1f;
        if ((glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) && (curr_obj_move == 3))
            source3.pos.y += 0.1f;
        if ((glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) && (curr_obj_move == 3))
            source3.pos.y -= 0.1f;

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

        //currPos[1].x = 20*sin(time / 4.0);
        //currPos[1].z = 4.0f + 20*sin(time / 4.0);

        //All Maps Creating
        if (preRenderBefore)
        {
            render.shadowMapGen();
            render.createEnvBRDFMod(BRDFShaders);
            render.createEnvFramebuffer();
            for (unsigned nCurr = 0; nCurr < 1; ++nCurr)
            {
                render.createEnvMapMod(nCurr);
                render.createAllEnvMapsMod2(prefShaders, currPos[nCurr], nCurr);
            }
            preRenderBefore = false;
        }

        //Shadow Process
        render.shadowMapProcess(source1.pos, shadowShaders);
        for (int i = 0; i < 3; ++i)
        {
            glm::mat4 model = glm::mat4(1.0f);
            glm::vec3 Pos = glm::vec3(currPos[i].x, currPos[i].y, currPos[i].z);
            model = glm::translate(model, Pos);
            model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
            shadowShaders.mat4Uniform("model", model);
            render.sphereRender();
        }

        /*for (int i = 0; i < 10; ++i)
        {
            glm::mat4 model = glm::mat4(1.0f);
            glm::vec3 Pos = glm::vec3(columnPos[i].x, columnPos[i].y, columnPos[i].z);
            model = glm::translate(model, Pos);
            model = glm::scale(model, glm::vec3(0.5, 2.0, 0.5));
            shadowShaders.mat4Uniform("model", model);
            render.cubeRender();
        }*/

        for (int i = 0; i < 10; ++i)
            for (int j = 0; j < 10; ++j)
            {
                glm::mat4 model = glm::mat4(1.0f);
                glm::vec3 columnCurr = glm::vec3(-4.0f + i * 4.0f, 0.0f, -32.0f + j * 8.0f);
                model = glm::translate(model, columnCurr);
                model = glm::scale(model, glm::vec3(0.5, 2.0, 0.5));
                shadowShaders.mat4Uniform("model", model);
                render.cubeRender();
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
            universal_prerender.bind_light_source(0, source1);
            universal_prerender.bind_light_source(1, source2);
            universal_prerender.bind_light_source(2, source3);

            for (unsigned nCurr = 0; nCurr < 1; ++nCurr)
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

                    ONShaders.UseShaderProgramm();
                    glm::mat4 model = glm::mat4(1.0f);
                    view = render.getCaptureView(i);

                    unsigned curr, currP;
                    for (unsigned i = 0; i < 3; ++i)
                    {
                        if (i != nCurr)
                        {
                            glm::mat4 model = glm::mat4(1.0f);
                            model = glm::translate(model, currPos[i] - viewPos);
                            //model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
                            universal_prerender.bind_matrices(model, view, proj, viewPos);
                            universal_prerender.bind_light_source(0, source1);
                            universal_prerender.bind_light_source(1, source2);
                            universal_prerender.bind_light_source(2, source3);
                            spheres[i].active_textures();
                            render.sphereRender();
                        }
                    }

                    /*PBRShaders.UseShaderProgramm();
                    for (int i = 0; i < 10; ++i)
                    {
                        glm::mat4 model = glm::mat4(1.0f);
                        glm::vec3 Pos = glm::vec3(columnPos[i].x, columnPos[i].y, columnPos[i].z);
                        model = glm::translate(model, Pos - viewPos);
                        model = glm::scale(model, glm::vec3(0.5, 2.0, 0.5));
                        columns.bind_matrices(model, view, proj, viewPos);
                        columns.bind_light_source(0, source1);
                        columns.bind_light_source(1, source2);
                        columns.bind_light_source(2, source3);
                        columns.active_textures();
                        render.cubeRender();
                    }*/

                    PBRShaders.UseShaderProgramm();
                    for (int i = 0; i < 10; ++i)
                        for (int j = 0; j < 10; ++j)
                        {
                            glm::mat4 model = glm::mat4(1.0f);
                            glm::vec3 Pos = glm::vec3(-4.0f + i * 4.0f, 0.0f, -32.0f + j * 8.0f);
                            model = glm::translate(model, Pos - viewPos);
                            model = glm::scale(model, glm::vec3(0.5, 4.0, 0.5));
                            columns.bind_matrices(model, view, proj, viewPos);
                            columns.bind_light_source(0, source1);
                            columns.bind_light_source(1, source2);
                            columns.bind_light_source(2, source3);
                            columns.active_textures();
                            render.cubeRender();
                        }

                    PBRShaders.UseShaderProgramm();
                    model = glm::mat4(1.0f);
                    model = glm::translate(model, platePos - viewPos);
                    model = glm::scale(model, glm::vec3(50.0f));
                    plate.bind_matrices(model, view, proj, viewPos);
                    plate.bind_light_source(0, source1);
                    plate.bind_light_source(1, source2);
                    plate.bind_light_source(2, source3);
                    plate.active_textures();
                    render.shadowMapActive(PBRShaders);
                    render.plateRender();

                    lightShaders.UseShaderProgramm();
                    model = glm::mat4(1.0f);
                    model = glm::translate(model, source1.pos - viewPos);
                    model = glm::scale(model, glm::vec3(0.2f));
                    light_render.bind_matrices(model, view, proj);
                    lightShaders.vec3Uniform("lightColor", lightColor1);
                    render.sphereRender();

                    lightShaders.UseShaderProgramm();
                    model = glm::mat4(1.0f);
                    model = glm::translate(model, source2.pos - viewPos);
                    model = glm::scale(model, glm::vec3(0.2f));
                    light_render.bind_matrices(model, view, proj);
                    lightShaders.vec3Uniform("lightColor", lightColor2);
                    render.sphereRender();

                    lightShaders.UseShaderProgramm();
                    model = glm::mat4(1.0f);
                    model = glm::translate(model, source3.pos - viewPos);
                    model = glm::scale(model, glm::vec3(0.2f));
                    light_render.bind_matrices(model, view, proj);
                    lightShaders.vec3Uniform("lightColor", lightColor3);
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
            //preRenderAfter = false;
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
        for (int i = 0; i < 1; ++i)
        {
            glm::mat4 model = glm::mat4(1.0f);
            glm::vec3 Pos = glm::vec3(currPos[i].x, currPos[i].y, currPos[i].z);
            model = glm::translate(model, Pos);
            model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));

            spheres[i].bind_matrices(model, view, proj, viewPos);
            spheres[i].bind_light_source(0, source1);
            spheres[i].bind_light_source(1, source2);
            spheres[i].bind_light_source(2, source3);

            spheres[i].active_textures();
            render.IBLEnvactiveMod2(IBLShaders, i);
            render.sphereRender();
        }

        ONShaders.UseShaderProgramm();
        for (int i = 1; i < 3; ++i)
        {
            glm::mat4 model = glm::mat4(1.0f);
            glm::vec3 Pos = glm::vec3(currPos[i].x, currPos[i].y, currPos[i].z);
            model = glm::translate(model, Pos);
            model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));

            spheres[i].bind_matrices(model, view, proj, viewPos);
            spheres[i].bind_light_source(0, source1);
            spheres[i].bind_light_source(1, source2);
            spheres[i].bind_light_source(2, source3);

            spheres[i].active_textures();
            render.sphereRender();
        }

        /*PBRShaders.UseShaderProgramm();
        for (int i = 0; i < 10; ++i)
        {
            glm::mat4 model = glm::mat4(1.0f);
            glm::vec3 Pos = glm::vec3(columnPos[i].x, columnPos[i].y, columnPos[i].z);
            model = glm::translate(model, Pos);
            model = glm::scale(model, glm::vec3(0.5, 2.0, 0.5));
            columns.bind_matrices(model, view, proj, viewPos);
            columns.bind_light_source(0, source1);
            columns.bind_light_source(1, source2);
            columns.bind_light_source(2, source3);
            columns.active_textures();
            render.shadowMapActive(PBRShaders);
            render.cubeRender();
        }*/

        ONShaders.UseShaderProgramm();
        for (int i = 0; i < 10; ++i)
            for (int j = 0; j < 10; ++j)
            {
                glm::mat4 model = glm::mat4(1.0f);
                glm::vec3 columnCurr = glm::vec3(-4.0f + i * 4.0f, 0.0f, -32.0f + j * 8.0f);
                model = glm::translate(model, columnCurr);
                model = glm::scale(model, glm::vec3(0.5, 4.0, 0.5));
                columns.bind_matrices(model, view, proj, viewPos);
                columns.bind_light_source(0, source1);
                columns.bind_light_source(1, source2);
                columns.bind_light_source(2, source3);
                columns.active_textures();
                render.cubeRender();
            }

        PBRShaders.UseShaderProgramm();
        model = glm::mat4(1.0f);
        model = glm::translate(model, platePos);
        model = glm::scale(model, glm::vec3(50.0f));
        plate.bind_matrices(model, view, proj, viewPos);
        plate.bind_light_source(0, source1);
        plate.bind_light_source(1, source2);
        plate.bind_light_source(2, source3);
        plate.active_textures();
        render.shadowMapActive(PBRShaders);
        render.plateRender();

        lightShaders.UseShaderProgramm();
        model = glm::mat4(1.0f);
        model = glm::translate(model, source1.pos);
        model = glm::scale(model, glm::vec3(0.05f));
        light_render.bind_matrices(model, view, proj);
        lightShaders.vec3Uniform("lightColor", lightColor1);
        render.sphereRender();

        lightShaders.UseShaderProgramm();
        model = glm::mat4(1.0f);
        model = glm::translate(model, source2.pos);
        model = glm::scale(model, glm::vec3(0.05f));
        light_render.bind_matrices(model, view, proj);
        lightShaders.vec3Uniform("lightColor", lightColor2);
        render.sphereRender();

        lightShaders.UseShaderProgramm();
        model = glm::mat4(1.0f);
        model = glm::translate(model, source3.pos);
        model = glm::scale(model, glm::vec3(0.05f));
        light_render.bind_matrices(model, view, proj);
        lightShaders.vec3Uniform("lightColor", lightColor3);
        render.sphereRender();

        //backShaders.UseShaderProgramm();
        //backShaders.mat4Uniform("view", view);
        //backShaders.mat4Uniform("proj", proj);
        //render.testCubeMapRender(1);

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
            avgTime = glfwGetTime();

        if (currFrame == 119)
            avgFrame = 60.0 / (glfwGetTime() - avgTime);

        if (currFrame == 1200)
        {
            std::cout << "Benchmark time: " << glfwGetTime() - firstFrameTime << std::endl;
            std::cout << "AVG fps: " << avgFrame << std::endl;
            std::cout << "AVG / 60 fps: " << avgFrame / 60.0 << std::endl;
            //glfwTerminate();
            //return 0;
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
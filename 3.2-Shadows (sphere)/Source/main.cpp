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

//Materials
glm::vec3 chrome_diff(0.4f, 0.4f, 0.4f);
glm::vec3 chrome_spec(0.774597f, 0.774597f, 0.774597f);
float chrome_shininess(0.6f);

glm::vec3 obsidian_diff(0.18275f, 0.17f, 0.22525f);
glm::vec3 obsidian_spec(0.332741f, 0.328634f, 0.346435f);
float obsidian_shininess(0.3f);

glm::vec3 bronze_diff(0.714f, 0.4284f, 0.18144f);
glm::vec3 bronze_spec(0.393548f, 0.271906f, 0.166721f);
float bronze_shininess(0.2f);

glm::vec3 wrubber_diff(0.5f, 0.5f, 0.5f);
glm::vec3 wrubber_spec(0.7f, 0.7f, 0.7f);
float wrubber_shininess(0.078125f);

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

int main()
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(1920, 1080, "OpenGL", NULL, NULL);
    glfwMakeContextCurrent(window);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);


    //Depth mapping for shadow

    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);

    unsigned int depthCubemap;
    glGenTextures(1, &depthCubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
    for (unsigned int i = 0; i < 6; ++i)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);

    //Shaders configure

    shader shaders("Shaders/VertexShadow2", "Shaders/ShadowColor2");
    shader lightShaders("Shaders/VertexLight", "Shaders/lightColor");
    shader mappingShaders("Shaders/VertexDepth", "Shaders/DepthColor", "Shaders/GeomDepth");
    objectsRender render;

    glm::vec3 lightPos1 = glm::vec3(0.0f, 5.0f, 0.0f);
    glm::vec3 lightColor1 = glm::vec3(1.0f, 1.0f, 1.0f);
    int currLight = 1;

    glm::vec3 currPos[] =
    { glm::vec3(0.0f,  0.0f, 0.0f), 
      glm::vec3(0.0f,  0.0f, 7.0f), 
      glm::vec3(2.0f,  0.0f, 0.0f),
      glm::vec3(7.0f,  0.0f, 0.0f), };

    shaders.UseShaderProgramm();
    shaders.IntUniform("shadowMap", 0);


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

        float near_plane = 1.0f;
        float far_plane = 25.0f;
        glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, near_plane, far_plane);
        std::vector<glm::mat4> shadowTransforms;
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos1, lightPos1 + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos1, lightPos1 + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos1, lightPos1 + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos1, lightPos1 + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos1, lightPos1 + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos1, lightPos1 + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));


        //First render for depth mapping

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);

        mappingShaders.UseShaderProgramm();

        for (unsigned int i = 0; i < 6; ++i)
            mappingShaders.mat4Uniform("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
        mappingShaders.FloatUniform("far_plane", far_plane);
        mappingShaders.vec3Uniform("lightPos", lightPos1);

        glm::mat4 plateModel = glm::mat4(1.0f);
        plateModel = glm::translate(plateModel, glm::vec3(0.0f, -1.0f, 0.0f));
        plateModel = glm::scale(plateModel, glm::vec3(50.0f, 50.0f, 50.0f));
        mappingShaders.mat4Uniform("model", plateModel);
        render.plateRender();

        for (int i = 0; i < 6; ++i)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
            glm::vec3 Pos = glm::vec3(currPos[0].x + 5 * sin(time + i), currPos[0].y, currPos[0].z + 5 * cos(time + i));
            model = glm::translate(model, Pos);
            mappingShaders.mat4Uniform("model", model);
            render.sphereRender();
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);


        glViewport(0, 0, 1920, 1080);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Camera and projective matrixes

        glm::mat4 proj = glm::mat4(1.0f);
        proj = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);
        glm::mat4 view = Camera.mat4getView();
        glm::vec3 viewPos = Camera.getCameraPos();


        //Main render

        glViewport(0, 0, 1920, 1080);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shaders.UseShaderProgramm();

        //Plate render

        plateModel = glm::mat4(1.0f);
        plateModel = glm::translate(plateModel, glm::vec3(0.0f, -1.0f, 0.0f));
        plateModel = glm::scale(plateModel, glm::vec3(50.0f, 50.0f, 50.0f));
        shaders.mat4Uniform("model", plateModel);
        shaders.mat4Uniform("proj", proj);
        shaders.mat4Uniform("view", view);
        shaders.FloatUniform("far_plane", far_plane);

        shaders.vec3Uniform("material.diffuse", chrome_diff);
        shaders.vec3Uniform("material.specular", chrome_spec);
        shaders.FloatUniform("material.shininess", chrome_shininess);

        shaders.vec3Uniform("lights[0].color", lightColor1);
        shaders.vec3Uniform("lights[0].pos", lightPos1);
        shaders.vec3Uniform("lights[0].atten", 1.0f, 0.09f, 0.032f);
        shaders.FloatUniform("lights[0].diff", 1.0f);
        shaders.FloatUniform("lights[0].spec", 0.9f);

        shaders.vec3Uniform("viewPos", viewPos);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);

        render.plateRender();
        
        //Columns render

        for (int i = 0; i < 6; ++i)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
            glm::vec3 Pos = glm::vec3(currPos[0].x + 5 * sin(time + i), currPos[0].y, currPos[0].z + 5 * cos(time + i));
            model = glm::translate(model, Pos);

            shaders.mat4Uniform("proj", proj);
            shaders.mat4Uniform("view", view);
            shaders.mat4Uniform("model", model);
            shaders.FloatUniform("far_plane", far_plane);

            shaders.vec3Uniform("material.diffuse", bronze_diff);
            shaders.vec3Uniform("material.specular", bronze_diff);
            shaders.FloatUniform("material.shininess", bronze_shininess);

            shaders.vec3Uniform("lights[0].color", lightColor1);
            shaders.vec3Uniform("lights[0].pos", lightPos1);
            shaders.vec3Uniform("lights[0].atten", 1.0f, 0.09f, 0.032f);
            shaders.FloatUniform("lights[0].diff", 1.0f);
            shaders.FloatUniform("lights[0].spec", 0.9f);

            shaders.vec3Uniform("viewPos", viewPos);
            render.sphereRender();
        }

        //Lighting render

        lightShaders.UseShaderProgramm();

        glm::mat4 lightModel1 = glm::mat4(1.0f);
        lightModel1 = glm::translate(lightModel1, lightPos1);
        lightModel1 = glm::scale(lightModel1, glm::vec3(0.05f));

        lightShaders.mat4Uniform("model", lightModel1);
        lightShaders.mat4Uniform("proj", proj);
        lightShaders.mat4Uniform("view", view);
        lightShaders.vec3Uniform("lightColor", lightColor1);

        render.cubeRender();

        glfwSwapBuffers(window);
        glfwPollEvents();

        //glm::vec3 cameraCurrPos = Camera.getCameraPos();
        //for (int i = 0; i < 3; ++i)
        //    std::cout << cameraCurrPos.x << " " << cameraCurrPos.y << " " << cameraCurrPos.z << " " << std::endl;
    }

    glfwTerminate();
    return 0;

}
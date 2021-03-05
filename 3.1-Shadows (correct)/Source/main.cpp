#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "shader.h"
#include "camera.h"
#include "objects.h"


camera Camera(glm::vec3(0.5f, 0.0f, -3.0f), glm::vec3(0.0f, 1.0f, 0.0f), 90.0f, 0.0f);
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

    unsigned int VBO, VBO_light, VBO_plate, VAO, VAO_light, VAO_plate;

    //VAO for lighting
    
    glGenVertexArrays(1, &VAO_light);
    glGenBuffers(1, &VBO_light);

    glBindVertexArray(VAO_light);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_light);
    glBufferData(GL_ARRAY_BUFFER, sizeof(objects::octahedron), objects::octahedron, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //VAO for plate

    glGenVertexArrays(1, &VAO_plate);
    glGenBuffers(1, &VBO_plate);

    glBindVertexArray(VAO_plate);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_plate);
    glBufferData(GL_ARRAY_BUFFER, sizeof(objects::plate), objects::plate, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //VAO for columns

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(objects::cube), objects::cube, GL_STATIC_DRAW);

    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


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

    glm::vec3 lightPos1 = glm::vec3(-1.0f, 1.0f, 2.0f);
    glm::vec3 lightColor1 = glm::vec3(1.0f, 1.0f, 1.0f);
    int currLight = 1;

    glm::vec3 currPos[] =
    { glm::vec3(0.0f,  0.0f, 2.0f), 
      glm::vec3(0.0f,  0.0f, 7.0f), };

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
        plateModel = glm::translate(plateModel, glm::vec3(0.0f, -1.0f, 2.0f));
        plateModel = glm::scale(plateModel, glm::vec3(2.0f, 2.0f, 3.0f));
        mappingShaders.mat4Uniform("model", plateModel);
        glBindVertexArray(VAO_plate);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        for (int i = 0; i < 2; ++i)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::scale(model, glm::vec3(0.5, 2.0, 0.5));
            model = glm::translate(model, currPos[i]);
            mappingShaders.mat4Uniform("model", model);
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
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
        plateModel = glm::translate(plateModel, glm::vec3(0.0f, -1.0f, 2.0f));
        plateModel = glm::scale(plateModel, glm::vec3(2.0f, 2.0f, 3.0f));
        shaders.mat4Uniform("model", plateModel);
        shaders.mat4Uniform("proj", proj);
        shaders.mat4Uniform("view", view);
        shaders.FloatUniform("far_plane", far_plane);

        shaders.vec3Uniform("material.diffuse", objects::chrome_diff);
        shaders.vec3Uniform("material.specular", objects::chrome_spec);
        shaders.FloatUniform("material.shininess", objects::chrome_shininess);

        shaders.vec3Uniform("lights[0].color", lightColor1);
        shaders.vec3Uniform("lights[0].pos", lightPos1);
        shaders.vec3Uniform("lights[0].atten", 1.0f, 0.22f, 0.20f);
        shaders.FloatUniform("lights[0].diff", 1.0f);
        shaders.FloatUniform("lights[0].spec", 0.9f);

        shaders.vec3Uniform("viewPos", viewPos);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);

        glBindVertexArray(VAO_plate);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        //Columns render

        for (int i = 0; i < 2; ++i)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::scale(model, glm::vec3(0.5, 2.0, 0.5));
            model = glm::translate(model, currPos[i]);

            shaders.mat4Uniform("proj", proj);
            shaders.mat4Uniform("view", view);
            shaders.mat4Uniform("model", model);
            shaders.FloatUniform("far_plane", far_plane);

            shaders.vec3Uniform("material.diffuse", objects::wrubber_diff);
            shaders.vec3Uniform("material.specular", objects::wrubber_diff);
            shaders.FloatUniform("material.shininess", objects::wrubber_shininess);

            shaders.vec3Uniform("lights[0].color", lightColor1);
            shaders.vec3Uniform("lights[0].pos", lightPos1);
            shaders.vec3Uniform("lights[0].atten", 1.0f, 0.22f, 0.20f);
            shaders.FloatUniform("lights[0].diff", 1.0f);
            shaders.FloatUniform("lights[0].spec", 0.9f);

            shaders.vec3Uniform("viewPos", viewPos);
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        //Lighting render

        lightShaders.UseShaderProgramm();

        glm::mat4 lightModel1 = glm::mat4(1.0f);
        lightModel1 = glm::translate(lightModel1, lightPos1);
        lightModel1 = glm::scale(lightModel1, glm::vec3(0.1f));

        lightShaders.mat4Uniform("model", lightModel1);
        lightShaders.mat4Uniform("proj", proj);
        lightShaders.mat4Uniform("view", view);
        lightShaders.vec3Uniform("lightColor", lightColor1);

        glBindVertexArray(VAO_light);
        glDrawArrays(GL_TRIANGLES, 0, 24);

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    glfwTerminate();
    return 0;

}
#include "objectsRender.h"

objectsRender::objectsRender()
{
    VAO = 0;
    VBO = 0;
    EBO = 0;
    sphereVert = sphereCreateVert(32);
    sphereInd = sphereCreateInd(32);
}

objectsRender::~objectsRender(){}


void objectsRender::cubeRender()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glDrawArrays(GL_TRIANGLES, 0, 36);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glBindVertexArray(0);
}

void objectsRender::quadRender()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glBindVertexArray(0);
}

void objectsRender::screenRender()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(screenVertices), screenVertices, GL_STATIC_DRAW);

    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glBindVertexArray(0);
}

std::vector<float> objectsRender::sphereCreateVert(unsigned divCount)
{
    std::vector<float> data;

    for (unsigned y = 0; y <= divCount; ++y)
    {
        for (unsigned x = 0; x <= divCount; ++x)
        {
            float xCurr = (float)x / divCount;
            float yCurr = (float)y / divCount;

            float xPos = cos(xCurr * 2.0f * PI) * sin(yCurr * PI);
            float yPos = cos(yCurr * PI);
            float zPos = sin(xCurr * 2.0f * PI) * sin(yCurr * PI);

            for (unsigned i = 0; i < 2; ++i)
            {
                data.push_back(xPos);
                data.push_back(yPos);
                data.push_back(zPos);
            }
            data.push_back(xPos);
            data.push_back(yPos);
        }
    }

    return data;
}

std::vector<unsigned> objectsRender::sphereCreateInd(unsigned divCount)
{
    std::vector<unsigned> data;
    bool oddRow = false;

    for (unsigned int y = 0; y < divCount; y += 2)
    {
        for (unsigned int x = 0; x <= divCount; ++x)
        {
            data.push_back(y * (divCount + 1) + x);
            data.push_back((y + 1) * (divCount + 1) + x);
        }
    }
    for (unsigned int y = 1; y < divCount; y += 2)
    {
        for (int x = divCount; x >= 0; --x)
        {
            data.push_back((y + 1) * (divCount + 1) + x);
            data.push_back(y * (divCount + 1) + x);
        }
    }

    return data;
}

void objectsRender::sphereRender()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sphereVert.size() * sizeof(float), &sphereVert[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereInd.size() * sizeof(unsigned int), &sphereInd[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));


    glDrawElements(GL_TRIANGLE_STRIP, sphereInd.size(), GL_UNSIGNED_INT, 0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glBindVertexArray(0);
}

void objectsRender::plateRender()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(plateVertices), plateVertices, GL_STATIC_DRAW);

    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}


unsigned int objectsRender::loadTexture(std::string path)
{
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
    return texture;
}

unsigned int objectsRender::loadHDR(std::string path)
{
    stbi_set_flip_vertically_on_load(true);
    unsigned int hdr;
    glGenTextures(1, &hdr);
    glBindTexture(GL_TEXTURE_2D, hdr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrComponents;
    float* hdrData = stbi_loadf(path.c_str(), &width, &height, &nrComponents, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, hdrData);

    stbi_image_free(hdrData);
    return hdr;
}

std::vector<unsigned> objectsRender::loadPBRtextures(std::string path)
{
    std::vector<unsigned> textures;
    textures.push_back(loadTexture(path + "/albedo.jpg"));
    textures.push_back(loadTexture(path + "/normal.jpg"));
    textures.push_back(loadTexture(path + "/metallic.jpg"));
    textures.push_back(loadTexture(path + "/roughness.jpg"));
    if (useAO)
        textures.push_back(loadTexture(path + "/ao.jpg"));
    return(textures);
}

void objectsRender::activePBRtextures(std::vector<unsigned> textures)
{
    if (useAO)
    {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textures[0]);;
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, textures[1]);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, textures[2]);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, textures[3]);
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, textures[4]);
    }
    else
    {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textures[0]);;
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, textures[1]);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, textures[2]);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, textures[3]);
    }
}

void objectsRender::activePBRmaps(shader pbrShader)
{
    pbrShader.UseShaderProgramm();
    pbrShader.IntUniform("albedoMap", 1);
    pbrShader.IntUniform("normalMap", 2);
    pbrShader.IntUniform("metallicMap", 3);
    pbrShader.IntUniform("roughnessMap", 4);

    pbrShader.IntUniform("irradianceMap", 5);
    pbrShader.IntUniform("prefilterMap", 6);
    pbrShader.IntUniform("brdfLUT", 7);
}

void objectsRender::bindPBRmaps(shader pbrShader)
{
    pbrShader.UseShaderProgramm();
    pbrShader.IntUniform("albedoMap", 1);
    pbrShader.IntUniform("normalMap", 2);
    pbrShader.IntUniform("metallicMap", 3);
    pbrShader.IntUniform("roughnessMap", 4);
}

void objectsRender::bindIBLmaps(shader iblShader)
{
    iblShader.IntUniform("irradianceMap", 5);
    iblShader.IntUniform("prefilterMap", 6);
    iblShader.IntUniform("brdfLUT", 7);
}

void objectsRender::initBuffers()
{
    glGenFramebuffers(1, &captureFBO);
    glGenRenderbuffers(1, &captureRBO);
}

void objectsRender::createSkybox(shader cubeShader, shader envShaders, char const* HDRpath)
{
    glGenFramebuffers(1, &captureFBO);
    glGenRenderbuffers(1, &captureRBO);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

    unsigned int hdrTexture = loadHDR(HDRpath);

    glGenTextures(1, &envCubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
    for (unsigned int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    cubeShader.UseShaderProgramm();
    cubeShader.IntUniform("rectMap", 0);
    cubeShader.mat4Uniform("proj", captureProjection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, hdrTexture);

    glViewport(0, 0, 512, 512);
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    for (unsigned int i = 0; i < 6; ++i)
    {
        cubeShader.mat4Uniform("view", captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        cubeRender();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    envShaders.UseShaderProgramm();
    envShaders.IntUniform("envMap", 0);
}

void objectsRender::skyboxRender()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
    cubeRender();
}

void objectsRender::envRender(shader envShaders)
{
    envShaders.UseShaderProgramm();
    envShaders.IntUniform("envMap", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
    cubeRender();
}

void objectsRender::IBLdiffuse(shader convShader)
{
    glGenTextures(1, &irradianceMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
    for (unsigned int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

    convShader.UseShaderProgramm();
    convShader.IntUniform("envMap", 0);
    convShader.mat4Uniform("proj", captureProjection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

    glViewport(0, 0, 32, 32);
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    for (unsigned int i = 0; i < 6; ++i)
    {
        convShader.mat4Uniform("view", captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        cubeRender();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void objectsRender::IBLpbrDiff(shader pbrShader)
{
    pbrShader.UseShaderProgramm();
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
}

void objectsRender::IBLspec(shader prefShader)
{
    glGenTextures(1, &prefilterMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
    for (unsigned int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    prefShader.UseShaderProgramm();
    prefShader.IntUniform("envMap", 0);
    prefShader.mat4Uniform("proj", captureProjection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    unsigned int maxMipLevels = 5;
    for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
    {
        unsigned int mipWidth = 128 * std::pow(0.5, mip);
        unsigned int mipHeight = 128 * std::pow(0.5, mip);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
        glViewport(0, 0, mipWidth, mipHeight);

        float roughness = (float)mip / (float)(maxMipLevels - 1);
        prefShader.FloatUniform("roughness", roughness);
        for (unsigned int i = 0; i < 6; ++i)
        {
            prefShader.mat4Uniform("view", captureViews[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilterMap, mip);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            cubeRender();
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void objectsRender::IBLbrdf(shader brdfShader)
{
    glGenTextures(1, &brdfLUTTexture);

    glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUTTexture, 0);

    glViewport(0, 0, 512, 512);
    brdfShader.UseShaderProgramm();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    quadRender();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void objectsRender::IBLpbrSpec(shader pbrShader)
{
    pbrShader.UseShaderProgramm();
    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
    glActiveTexture(GL_TEXTURE7);
    glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
}

void objectsRender::IBLactive(shader pbrShader)
{
    pbrShader.UseShaderProgramm();
    if (useAO)
    {
        glActiveTexture(GL_TEXTURE6);
        glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
        glActiveTexture(GL_TEXTURE7);
        glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
        glActiveTexture(GL_TEXTURE8);
        glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
    }
    else
    {
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
        glActiveTexture(GL_TEXTURE6);
        glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
        glActiveTexture(GL_TEXTURE7);
        glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
    }
}


void objectsRender::createScreenFramebuffer()
{
    glGenFramebuffers(1, &screenFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, screenFramebuffer);

    glGenTextures(1, &textureBuffer);
    glBindTexture(GL_TEXTURE_2D, textureBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, scrWidth, scrHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureBuffer, 0);

    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, scrWidth, scrHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void objectsRender::deleteScreenFramebuffer()
{
    glDeleteRenderbuffers(1, &rbo);
    glDeleteFramebuffers(1, &screenFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void objectsRender::sideInit()
{
    glBindFramebuffer(GL_FRAMEBUFFER, screenFramebuffer);
    glEnable(GL_DEPTH_TEST);
}

void objectsRender::sideRender(shader screenShader)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    screenShader.UseShaderProgramm();
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textureBuffer);
    screenRender();
}


void objectsRender::setMapSizes(unsigned size1, unsigned size2)
{
    mapSizes[0] = size1;
    mapSizes[1] = size2;
}

void objectsRender::createMapSet(unsigned k)
{
    for (unsigned i = 0; i < k; ++i)
        Maps.push_back({ 0,0,0,0 });
}

void objectsRender::setCaptureViews(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f))
{
    glm::mat4 captureViews[6] =
    {
        glm::lookAt(pos, pos + glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(pos, pos + glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(pos, pos + glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
        glm::lookAt(pos, pos + glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
        glm::lookAt(pos, pos + glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(pos, pos + glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
    };
}

glm::mat4 objectsRender::getCaptureView(unsigned n)
{
    return captureViews[n];
}


void objectsRender::createEnvFramebuffer()
{
    glGenFramebuffers(1, &screenFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, screenFramebuffer);

    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, mapSizes[0], mapSizes[1]);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void objectsRender::createEnvMap(unsigned nObj)
{
    glBindFramebuffer(GL_FRAMEBUFFER, screenFramebuffer);
    glEnable(GL_DEPTH_TEST);
    glGenTextures(1, &Maps[nObj][0]);
    glBindTexture(GL_TEXTURE_CUBE_MAP, Maps[nObj][0]);

    for (unsigned int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, mapSizes[0], mapSizes[1], 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void objectsRender::saveEnvMapSide(unsigned nSide, unsigned nObj)
{
    glViewport(0, 0, mapSizes[0], mapSizes[1]);
    glBindFramebuffer(GL_FRAMEBUFFER, screenFramebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + nSide, Maps[nObj][0], 0);
}

void objectsRender::createEnvDiffuse(shader convShader, glm::vec3 viewPos, unsigned nObj)
{
    glGenTextures(1, &Maps[nObj][1]);
    glBindTexture(GL_TEXTURE_CUBE_MAP, Maps[nObj][1]);
    for (unsigned int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, (mapSizes[0] / 4), (mapSizes[1] / 4), 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, (mapSizes[0] / 4), (mapSizes[1] / 4));

    convShader.UseShaderProgramm();
    convShader.IntUniform("envMap", 0);
    convShader.mat4Uniform("proj", captureProjection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, Maps[nObj][0]);

    glViewport(0, 0, (mapSizes[0] / 4), (mapSizes[1] / 4));
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    setCaptureViews(viewPos);

    for (unsigned int i = 0; i < 6; ++i)
    {
        convShader.mat4Uniform("view", captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, Maps[nObj][1], 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        cubeRender();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void objectsRender::createEnvSpec(shader prefShader, glm::vec3 viewPos, unsigned nObj)
{
    glGenTextures(1, &Maps[nObj][2]);
    glBindTexture(GL_TEXTURE_CUBE_MAP, Maps[nObj][2]);
    for (unsigned int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, (mapSizes[0] / 4), (mapSizes[1] / 4), 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    prefShader.UseShaderProgramm();
    prefShader.IntUniform("envMap", 0);
    prefShader.mat4Uniform("proj", captureProjection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, Maps[nObj][0]);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    unsigned int maxMipLevels = 5;
    for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
    {
        unsigned int mipWidth = (mapSizes[0] / 4) * std::pow(0.5, mip);
        unsigned int mipHeight = (mapSizes[1] / 4) * std::pow(0.5, mip);

        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
        glViewport(0, 0, mipWidth, mipHeight);

        float roughness = (float)mip / (float)(maxMipLevels - 1);
        prefShader.FloatUniform("roughness", roughness);
        setCaptureViews(viewPos);

        for (unsigned int i = 0; i < 6; ++i)
        {
            prefShader.mat4Uniform("view", captureViews[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, Maps[nObj][2], mip);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            cubeRender();
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void objectsRender::createEnvBRDF(shader brdfShader, glm::vec3 viewPos, unsigned nObj)
{
    glGenTextures(1, &Maps[nObj][3]);

    glBindTexture(GL_TEXTURE_2D, Maps[nObj][3]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, (mapSizes[0] / 2), (mapSizes[1] / 2), 0, GL_RG, GL_FLOAT, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mapSizes[0] / 2, mapSizes[1] / 2);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Maps[nObj][3], 0);

    glViewport(0, 0, (mapSizes[0] / 2), (mapSizes[1] / 2));
    brdfShader.UseShaderProgramm();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    quadRender();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void objectsRender::IBLEnvactive(shader pbrShader, unsigned nObj)
{
    pbrShader.UseShaderProgramm();
    if (useAO)
    {
        glActiveTexture(GL_TEXTURE6);
        glBindTexture(GL_TEXTURE_CUBE_MAP, Maps[nObj][1]);
        glActiveTexture(GL_TEXTURE7);
        glBindTexture(GL_TEXTURE_CUBE_MAP, Maps[nObj][2]);
        glActiveTexture(GL_TEXTURE8);
        glBindTexture(GL_TEXTURE_2D, Maps[nObj][3]);
    }
    else
    {
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_CUBE_MAP, Maps[nObj][1]);
        glActiveTexture(GL_TEXTURE6);
        glBindTexture(GL_TEXTURE_CUBE_MAP, Maps[nObj][2]);
        glActiveTexture(GL_TEXTURE7);
        glBindTexture(GL_TEXTURE_2D, Maps[nObj][3]);
    }
}

void objectsRender::testCubeMapRender(unsigned nObj)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, Maps[nObj][0]);
    cubeRender();
}

void objectsRender::createAllEnvMaps(shader convShader, shader prefShader, shader brdfShader, glm::vec3 viewPos, unsigned nObj)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    createEnvDiffuse(convShader, viewPos, nObj);
    createEnvSpec(prefShader, viewPos, nObj);
    createEnvBRDF(brdfShader, viewPos, nObj);
}


void objectsRender::shadowMapCreate(glm::vec3 lightPos, shader shadowShader)
{
    glGenFramebuffers(1, &shadowFBO);

    glGenTextures(1, &shadowMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, shadowMap);
    for (unsigned int i = 0; i < 6; ++i)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, mapSizes[0], mapSizes[1], 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float near_plane = 0.1f;
    float far_plane = 100.0f;
    glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)mapSizes[0] / (float)mapSizes[1], near_plane, far_plane);
    std::vector<glm::mat4> shadowTransforms;

    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

    glViewport(0, 0, mapSizes[0], mapSizes[1]);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
    glClear(GL_DEPTH_BUFFER_BIT);

    shadowShader.UseShaderProgramm();

    for (unsigned int i = 0; i < 6; ++i)
        shadowShader.mat4Uniform("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
    shadowShader.FloatUniform("far_plane", far_plane);
    shadowShader.vec3Uniform("lightPos", lightPos);
}

void objectsRender::shadowMapBind(shader pbrShader)
{
    pbrShader.UseShaderProgramm();
    pbrShader.IntUniform("shadowMap", 8);
}

void objectsRender::shadowMapActive(shader pbrShader)
{
    pbrShader.UseShaderProgramm();
    glActiveTexture(GL_TEXTURE8);
    glBindTexture(GL_TEXTURE_CUBE_MAP, shadowMap);
}


void objectsRender::addTextureToPack(std::vector<unsigned> texture)
{
    TexturePack.push_back(texture);
}

void objectsRender::objectPBRsphere(shader pbrShader, glm::vec3 Pos, std::vector<unsigned> textures)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, Pos);
    model = glm::scale(model, glm::vec3(0.8f));
    pbrShader.mat4Uniform("model", model);
    activePBRtextures(textures);
    sphereRender();
}

void objectsRender::objectIBLsphere(shader IBLShader, glm::vec3 Pos, std::vector<unsigned> textures)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, Pos);
    model = glm::scale(model, glm::vec3(0.8f));
    IBLShader.mat4Uniform("model", model);
    activePBRtextures(textures);
    IBLactive(IBLShader);
    sphereRender();
}

void objectsRender::objectIBLEnvsphere(shader IBLShader, unsigned nObj, glm::vec3 Pos, std::vector<unsigned> textures)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, Pos);
    model = glm::scale(model, glm::vec3(0.8f));
    IBLShader.mat4Uniform("model", model);
    activePBRtextures(textures);
    IBLEnvactive(IBLShader, nObj);
    sphereRender();
}

void objectsRender::objectPreRender(unsigned nCurr, std::vector<glm::vec3> positions,
                                    std::vector<std::vector<unsigned>> texturePack,
                                    shader pbrShader, shader backShader,
                                    shader convShader, shader prefShader, shader BRDFShader)
{
    unsigned count = positions.size();
    createEnvFramebuffer();
    createEnvMap(nCurr);
    setCaptureViews(positions[nCurr]);

    glm::mat4 view;
    glm::mat4 proj = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    glm::vec3 viewPos = positions[nCurr];

    for (unsigned i = 0; i < 6; ++i)
    {
        sideInit();
        saveEnvMapSide(i, nCurr);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        pbrShader.UseShaderProgramm();

        glm::mat4 model = glm::mat4(1.0f);
        view = getCaptureView(i);
      
        pbrShader.mat4Uniform("view", view);
        pbrShader.mat4Uniform("proj", proj);
        pbrShader.vec3Uniform("viewPos", viewPos);

        unsigned curr;
        for (unsigned i = 1; i < count; ++i)
        {
            curr = (i + nCurr) % count;
            objectIBLsphere(pbrShader, positions[curr] - viewPos, texturePack[curr]);
        }
                              
        backShader.UseShaderProgramm();
        backShader.mat4Uniform("view", view);
        backShader.mat4Uniform("proj", proj);
        envRender(backShader);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    createEnvDiffuse(convShader, positions[nCurr], nCurr);
    createEnvSpec(prefShader, positions[nCurr], nCurr);
    createEnvBRDF(BRDFShader, positions[nCurr], nCurr);
}

void objectsRender::objectPBRmodel(shader PBRShader, glm::vec3 Pos, model obj)
{
    PBRShader.UseShaderProgramm();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, Pos);
    PBRShader.mat4Uniform("model", model);
    obj.Draw(PBRShader);
}

void objectsRender::objectPBRAllmodels(shader PBRShader, std::vector<glm::vec3> positions, std::vector<model> objects)
{
    unsigned count = positions.size();
    PBRShader.UseShaderProgramm();
    for (unsigned i = 0; i < count; ++i)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, positions[i]);
        PBRShader.mat4Uniform("model", model);
        objects[i].Draw(PBRShader);
    }
}

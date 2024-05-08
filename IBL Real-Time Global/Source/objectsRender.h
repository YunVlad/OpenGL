#ifndef OBJECTSRENDER_H
#define OBJECTSRENDER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"

#include "shader.h"
//#include "model.h"
#include <vector>
#include <iostream>
#include <string>


class objectsRender
{
    enum objectType
    {
        SPHERE = 0,
        MODEL = 1,
        CUBE = 2,
        PLATE = 3
    };

public:
	objectsRender();
    ~objectsRender();


    //Objects part
    void quadRender();
	void cubeRender();
    void screenRender();
    void plateRender();
    
    void sphereRender();
    std::vector<float> sphereCreateVert(unsigned divCount);
    std::vector<unsigned> sphereCreateInd(unsigned divCount);


    //Texture part
    unsigned int loadTexture(std::string path);
    unsigned int loadHDR(std::string path);
    std::vector<unsigned> loadPBRtextures(std::string path);
    std::vector<unsigned> loadAlbedotexture(std::string path);
    void activePBRtextures(std::vector<unsigned> textures);
    void activeAlbedotexture(std::vector<unsigned> textures);
    void setAOtrue();
    void activePBRmaps(shader pbrShader);
    void bindPBRmaps(shader pbrShader);
    void bindIBLmaps(shader iblShader);


    //IBL part
    void initBuffers();
    void createSkybox(shader cubeShader, shader envShaders, char const* HDRpath);
    void skyboxRender();
    void envRender(shader envShaders);

    void IBLdiffuse(shader convShader); 
    void IBLspec(shader prefShader);
    void IBLbrdf(shader brdfShader);
    void IBLpbrDiff(shader pbrShader);
    void IBLpbrSpec(shader pbrShader);
    void IBLactive(shader pbrShader);


    //Screen Framebuffer part
    void createScreenFramebuffer();
    void deleteScreenFramebuffer();
    void sideInit();
    void sideRender(shader screenShader);


    //EnvMap Part
    void createMapSet(unsigned);
    void setCaptureViews(glm::vec3 pos);
    glm::mat4 getCaptureView(unsigned n);

    void setMapSizes(unsigned size1, unsigned size2);
    void createEnvFramebuffer();
    void createEnvMap(unsigned nObj);
    void saveEnvMapSide(unsigned nSide, unsigned nObj);
    void createEnvDiffuse(shader convShader, glm::vec3 viewPos, unsigned nObj);
    void createEnvSpec(shader prefShader, glm::vec3 viewPos, unsigned nObj);
    void createEnvBRDF(shader brdfShader, glm::vec3 viewPos, unsigned nObj);
    void IBLEnvactive(shader pbrShader, unsigned nObj);
    void testCubeMapRender(unsigned nObj);
    void createAllEnvMaps(shader convShader, shader prefShader, shader brdfShader, glm::vec3 viewPos, unsigned nObj);
    void deleteEnv(unsigned nObj);


    //Shadow Map Part
    void shadowMapGen();
    void shadowMapProcess(glm::vec3 lightPos, shader shadowShader);
    void shadowMapBind(shader pbrShader);
    void shadowMapActive(shader pbrShader);


    //Objects-render part
    void addTextureToPack(std::vector<unsigned>);

    void objectPBRsphere(shader IBLShader, glm::vec3 Pos, std::vector<unsigned> textures);
    void objectIBLsphere(shader IBLShader, glm::vec3 Pos, std::vector<unsigned> textures);
    void objectIBLEnvsphere(shader IBLShader, unsigned nObj, glm::vec3 Pos, std::vector<unsigned> textures);
    void objectPreRender(unsigned nCurr, std::vector<glm::vec3> positions,
                         std::vector<std::vector<unsigned>> texturePack,
                         shader pbrShader, shader backShader,
                         shader convShader, shader prefShader, shader BRDFShader);

    void createMapSetMod(unsigned);
    void createEnvMapMod(unsigned nObj);
    void saveEnvMapSideMod(unsigned nSide, unsigned nObj);
    void createEnvDiffuseMod(shader convShader, glm::vec3 viewPos, unsigned nObj);
    void createEnvDiffuseMod2(shader convShader);
    void createEnvSpecMod(shader prefShader, glm::vec3 viewPos, unsigned nObj);
    void createEnvBRDFMod(shader brdfShader);
    void createAllEnvMapsMod(shader convShader, shader prefShader, glm::vec3 viewPos, unsigned nObj);
    void createAllEnvMapsMod2(shader prefShader, glm::vec3 viewPos, unsigned nObj);
    void IBLEnvactiveMod(shader pbrShader, unsigned nObj);
    void IBLEnvactiveMod2(shader pbrShader, unsigned nObj);
 //   void objectPBRmodel(shader PBRShader, glm::vec3 Pos, model obj);
//    void objectPBRAllmodels(shader PBRShader, std::vector<glm::vec3> positions, std::vector<model> objects);


private:
    int scrWidth = 1920, scrHeight = 1080;
    bool useAO = false;
	unsigned VAO, VBO, EBO;
    unsigned captureFBO, captureRBO;
    unsigned EnvFBO, EnvRBO;
    unsigned envCubemap, irradianceMap, prefilterMap, brdfLUTTexture;

    unsigned screenFramebuffer;
    unsigned textureBuffer;
    unsigned rbo;

    unsigned shadowMap;
    unsigned shadowFBO;

    unsigned mapSizes[2] = { 256, 256 };
    std::vector<std::vector<unsigned>> Maps;
    std::vector<std::vector<unsigned>> MapsMod;
    std::vector<std::vector<unsigned>> TexturePack;
    

    float PI = 3.14159;

    std::vector<float> sphereVert;
    std::vector<unsigned> sphereInd;

    const float cubeVertices[356] =
    {
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, 
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
             1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,      
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,

            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
             1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,

            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
             1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,         
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
             1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,    

            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
             1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
            -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,

            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
             1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
             1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,    
             1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, 
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, 
            -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f          
    };

    const float plateVertices[48] =
    {
            -1.0f, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f, 0.0f, 10.0f,
            -1.0f, 0.0f,-1.0f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
             1.0f, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f, 10.0f, 10.0f,

            -1.0f, 0.0f,-1.0f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
             1.0f, 0.0f,-1.0f,  0.0f, 1.0f, 0.0f, 10.0f, 0.0f,
             1.0f, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f, 10.0f, 10.0f,
    };

    const float quadVertices[20] =
    {
       -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
       -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };

    const float screenVertices[24] =
    {
       -1.0f,  1.0f,  0.0f, 1.0f,
       -1.0f, -1.0f,  0.0f, 0.0f,
        1.0f, -1.0f,  1.0f, 0.0f,

       -1.0f,  1.0f,  0.0f, 1.0f,
        1.0f, -1.0f,  1.0f, 0.0f,
        1.0f,  1.0f,  1.0f, 1.0f,
    };

    glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    glm::mat4 captureViews[6] =
    {
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
    };

};

#endif //OBJECTSRENDER_H

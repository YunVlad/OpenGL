#include "objectsRender.h"

objectsRender::objectsRender()
{
    VAO = 0;
    VBO = 0;
    EBO = 0;
    sphereVert = sphereCreateVert(32);
    sphereInd = sphereCreateInd(32);
}

void objectsRender::cubeRender()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_TRIANGLES, 0, 36);
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));


    glDrawElements(GL_TRIANGLE_STRIP, sphereInd.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void objectsRender::plateRender()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(plateVertices), plateVertices, GL_STATIC_DRAW);

    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

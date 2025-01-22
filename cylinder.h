#ifndef cylinder_h
#define cylinder_h

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include <cmath>

class Cylinder {
public:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
    float TXmin = 0.0f;
    float TXmax = 1.0f;
    float TYmin = 0.0f;
    float TYmax = 1.0f;
    unsigned int diffuseMap;
    unsigned int specularMap;

    Cylinder(int sectors = 36)
    {
        setUpCylinderVertexDataAndConfigureVertexAttribute(sectors);
    }

    Cylinder(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shiny, int sectors = 36)
    {
        this->ambient = amb;
        this->diffuse = diff;
        this->specular = spec;
        this->shininess = shiny;
        setUpCylinderVertexDataAndConfigureVertexAttribute(sectors);
    }

    Cylinder(unsigned int dMap, unsigned int sMap, float shiny, float textureXmin, float textureYmin, float textureXmax, float textureYmax, int sectors = 36)
    {
        this->diffuseMap = dMap;
        this->specularMap = sMap;
        this->shininess = shiny;
        this->TXmin = textureXmin;
        this->TYmin = textureYmin;
        this->TXmax = textureXmax;
        this->TYmax = textureYmax;
        setUpCylinderVertexDataAndConfigureVertexAttribute(sectors);
    }

    ~Cylinder()
    {
        glDeleteVertexArrays(1, &cylinderVAO);
        glDeleteVertexArrays(1, &lightCylinderVAO);
        glDeleteVertexArrays(1, &lightTexCylinderVAO);
        glDeleteBuffers(1, &cylinderVBO);
        glDeleteBuffers(1, &cylinderEBO);
    }

    void drawCylinderWithTexture(Shader& lightingShaderWithTexture, glm::mat4 model = glm::mat4(1.0f))
    {
        lightingShaderWithTexture.use();

        lightingShaderWithTexture.setInt("material.diffuse", 0);
        lightingShaderWithTexture.setInt("material.specular", 1);
        lightingShaderWithTexture.setFloat("material.shininess", this->shininess);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->diffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, this->specularMap);

        lightingShaderWithTexture.setMat4("model", model);

        glBindVertexArray(lightTexCylinderVAO);
        glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
    }

    void drawCylinderWithMaterialisticProperty(Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f))
    {
        lightingShader.use();

        lightingShader.setVec3("material.ambient", this->ambient);
        lightingShader.setVec3("material.diffuse", this->diffuse);
        lightingShader.setVec3("material.specular", this->specular);
        lightingShader.setFloat("material.shininess", this->shininess);

        lightingShader.setMat4("model", model);

        glBindVertexArray(lightCylinderVAO);
        glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
    }

    void drawCylinder(Shader& shader, glm::mat4 model = glm::mat4(1.0f), float r = 1.0f, float g = 1.0f, float b = 1.0f)
    {
        shader.use();
        shader.setVec3("color", glm::vec3(r, g, b));
        shader.setMat4("model", model);

        glBindVertexArray(cylinderVAO);
        glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
    }

private:
    unsigned int cylinderVAO;
    unsigned int lightCylinderVAO;
    unsigned int lightTexCylinderVAO;
    unsigned int cylinderVBO;
    unsigned int cylinderEBO;
    int numIndices;

    void setUpCylinderVertexDataAndConfigureVertexAttribute(int sectors)
    {
        std::vector<float> vertices;
        std::vector<unsigned int> indices;
        const float radius = 0.5f;
        const float height = 1.0f;

        // Top and bottom center vertices
        vertices.push_back(0.0f); vertices.push_back(height / 2); vertices.push_back(0.0f);  // normal
        vertices.push_back(0.0f); vertices.push_back(1.0f); vertices.push_back(0.0f);      // texture
        vertices.push_back(0.5f); vertices.push_back(0.5f);

        vertices.push_back(0.0f); vertices.push_back(-height / 2); vertices.push_back(0.0f); // normal
        vertices.push_back(0.0f); vertices.push_back(-1.0f); vertices.push_back(0.0f);     // texture
        vertices.push_back(0.5f); vertices.push_back(0.5f);

        // Add vertices for the sides
        for (int i = 0; i <= sectors; i++) {
            float angle = 2.0f * 3.1416f * float(i) / float(sectors);
            float x = radius * cos(angle);
            float z = radius * sin(angle);
            float u = float(i) / float(sectors);

            // Top rim
            vertices.push_back(x);
            vertices.push_back(height / 2);
            vertices.push_back(z);
            vertices.push_back(0.0f);
            vertices.push_back(1.0f);
            vertices.push_back(0.0f);
            vertices.push_back(u);
            vertices.push_back(1.0f);

            // Bottom rim
            vertices.push_back(x);
            vertices.push_back(-height / 2);
            vertices.push_back(z);
            vertices.push_back(0.0f);
            vertices.push_back(-1.0f);
            vertices.push_back(0.0f);
            vertices.push_back(u);
            vertices.push_back(0.0f);

            // Side
            vertices.push_back(x);
            vertices.push_back(height / 2);
            vertices.push_back(z);
            vertices.push_back(x);
            vertices.push_back(0.0f);
            vertices.push_back(z);
            vertices.push_back(u);
            vertices.push_back(1.0f);

            vertices.push_back(x);
            vertices.push_back(-height / 2);
            vertices.push_back(z);
            vertices.push_back(x);
            vertices.push_back(0.0f);
            vertices.push_back(z);
            vertices.push_back(u);
            vertices.push_back(0.0f);
        }

        // Generate indices
        for (int i = 0; i < sectors; i++) {
            int baseIndex = 2 + (i * 4);

            // Top face
            indices.push_back(0);
            indices.push_back(baseIndex);
            indices.push_back(baseIndex + 4);

            // Bottom face
            indices.push_back(1);
            indices.push_back(baseIndex + 1);
            indices.push_back(baseIndex + 5);

            // Side faces
            indices.push_back(baseIndex + 2);
            indices.push_back(baseIndex + 3);
            indices.push_back(baseIndex + 6);

            indices.push_back(baseIndex + 3);
            indices.push_back(baseIndex + 7);
            indices.push_back(baseIndex + 6);
        }

        numIndices = indices.size();

        glGenVertexArrays(1, &cylinderVAO);
        glGenVertexArrays(1, &lightCylinderVAO);
        glGenVertexArrays(1, &lightTexCylinderVAO);
        glGenBuffers(1, &cylinderVBO);
        glGenBuffers(1, &cylinderEBO);

        glBindVertexArray(lightTexCylinderVAO);
        glBindBuffer(GL_ARRAY_BUFFER, cylinderVBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cylinderEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindVertexArray(lightCylinderVAO);
        glBindBuffer(GL_ARRAY_BUFFER, cylinderVBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cylinderEBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(cylinderVAO);
        glBindBuffer(GL_ARRAY_BUFFER, cylinderVBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cylinderEBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }
};

#endif /* cylinder_h */
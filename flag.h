//
//  flag.h
//  DynamicFlag
//
//  Created by Nazirul Hasan on 4/10/23.
//

#ifndef flag_h
#define flag_h

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"

class Flag {
public:
    // materialistic property
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    // texture property
    unsigned int diffuseMap;
    unsigned int specularMap;
    float shininess;

    // waving properties
    float waveSpeed;
    float waveAmplitude;

    // constructors
    Flag() {
        waveSpeed = 1.0f;
        waveAmplitude = 0.1f;
        setUpFlagVertexDataAndConfigureVertexAttributes();
    }

    Flag(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shiny, float waveAmp, float waveSpd)
        : ambient(amb), diffuse(diff), specular(spec), shininess(shiny),
        waveAmplitude(waveAmp), waveSpeed(waveSpd) {
        setUpFlagVertexDataAndConfigureVertexAttributes();
    }

    Flag(unsigned int dMap, unsigned int sMap, float shiny, float waveAmp, float waveSpd)
        : diffuseMap(dMap), specularMap(sMap), shininess(shiny),
        waveAmplitude(waveAmp), waveSpeed(waveSpd) {
        setUpFlagVertexDataAndConfigureVertexAttributes();
    }

    // destructor
    ~Flag() {
        glDeleteVertexArrays(1, &flagVAO);
        glDeleteBuffers(1, &flagVBO);
        glDeleteBuffers(1, &flagEBO);
    }

    // draw function
    void drawFlag(Shader& shader, glm::mat4 model, float time) {
        shader.use();

        // set wave uniform
        shader.setFloat("waveAmplitude", waveAmplitude);
        shader.setFloat("waveSpeed", waveSpeed);
        shader.setFloat("time", time);

        // set material properties
        shader.setVec3("material.ambient", ambient);
        shader.setVec3("material.diffuse", diffuse);
        shader.setVec3("material.specular", specular);
        shader.setFloat("material.shininess", shininess);

        // bind texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        // set transformation
        shader.setMat4("model", model);

        glBindVertexArray(flagVAO);
        glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);
    }

private:
    unsigned int flagVAO, flagVBO, flagEBO;
    int indicesCount;

    void setUpFlagVertexDataAndConfigureVertexAttributes() {
        // Example flag vertices and indices for a simple grid
        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        const int widthSegments = 20;
        const int heightSegments = 10;
        const float width = 1.0f;
        const float height = 0.5f;

        for (int y = 0; y <= heightSegments; ++y) {
            for (int x = 0; x <= widthSegments; ++x) {
                float u = (float)x / widthSegments;
                float v = (float)y / heightSegments;
                vertices.push_back(width * (u - 0.5f));
                vertices.push_back(height * (v - 0.5f));
                vertices.push_back(0.0f); // z-coordinate
                vertices.push_back(u);    // texture u
                vertices.push_back(v);    // texture v
            }
        }

        for (int y = 0; y < heightSegments; ++y) {
            for (int x = 0; x < widthSegments; ++x) {
                int topLeft = y * (widthSegments + 1) + x;
                int bottomLeft = (y + 1) * (widthSegments + 1) + x;

                indices.push_back(topLeft);
                indices.push_back(bottomLeft);
                indices.push_back(topLeft + 1);

                indices.push_back(topLeft + 1);
                indices.push_back(bottomLeft);
                indices.push_back(bottomLeft + 1);
            }
        }

        indicesCount = indices.size();

        // OpenGL setup
        glGenVertexArrays(1, &flagVAO);
        glGenBuffers(1, &flagVBO);
        glGenBuffers(1, &flagEBO);

        glBindVertexArray(flagVAO);

        glBindBuffer(GL_ARRAY_BUFFER, flagVBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, flagEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // texture coordinate attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }
};

#endif /* flag_h */

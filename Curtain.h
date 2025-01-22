#ifndef CURTAIN_H
#define CURTAIN_H

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include "shader.h"

class Curtain {
public:
    Curtain(unsigned int textureID, int waveSegments = 50, float waveAmplitude = 0.2f)
        : textureID(textureID), waveSegments(waveSegments), waveAmplitude(waveAmplitude) {
        setUpCurtainVertexData();
    }

    ~Curtain() {
        glDeleteVertexArrays(1, &curtainVAO);
        glDeleteBuffers(1, &curtainVBO);
        glDeleteBuffers(1, &curtainEBO);
    }

    void draw(Shader& shader, glm::mat4 model = glm::mat4(1.0f)) {
        shader.use();

        shader.setInt("material.texture", 0);
        shader.setMat4("model", model);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glBindVertexArray(curtainVAO);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

private:
    unsigned int curtainVAO, curtainVBO, curtainEBO;
    unsigned int textureID;
    int waveSegments;
    float waveAmplitude;

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    void setUpCurtainVertexData() {
        float depth = 1.0f;   // Curtain depth (z-axis)
        float height = 2.0f;  // Curtain height
        float segmentDepth = depth / waveSegments;

        // Generate vertices
        for (int y = 0; y <= 1; ++y) {  // 0 for bottom, 1 for top
            for (int i = 0; i <= waveSegments; ++i) {
                float z = -0.5f + i * segmentDepth; // Now varies along z-axis
                float x = (y == 0) ? 0.0f : waveAmplitude * sin(i * 2.0f * glm::pi<float>() / waveSegments);
                float texX = static_cast<float>(i) / waveSegments;
                float texY = static_cast<float>(y);
                vertices.insert(vertices.end(), { x, y * height - height / 2.0f, z, texX, texY });
            }
        }

        // Generate indices
        for (int i = 0; i < waveSegments; ++i) {
            indices.push_back(i);
            indices.push_back(i + waveSegments + 1);
            indices.push_back(i + waveSegments + 2);

            indices.push_back(i);
            indices.push_back(i + waveSegments + 2);
            indices.push_back(i + 1);
        }

        // VAO, VBO, EBO setup
        glGenVertexArrays(1, &curtainVAO);
        glGenBuffers(1, &curtainVBO);
        glGenBuffers(1, &curtainEBO);

        glBindVertexArray(curtainVAO);

        glBindBuffer(GL_ARRAY_BUFFER, curtainVBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, curtainEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        // Vertex attributes
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
    }
};

#endif

//
//  curvy_cube.h
//  test
//
//  Created by Nazirul Hasan on 4/10/23.
//

#ifndef curvy_cube_h
#define curvy_cube_h

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"

using namespace std;

class CurvyCube {
public:

    // Material properties
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    // Texture properties
    float TXmin = 0.0f;
    float TXmax = 1.0f;
    float TYmin = 0.0f;
    float TYmax = 1.0f;
    unsigned int diffuseMap;
    unsigned int specularMap;

    // Shininess
    float shininess;

    // Constructors
    CurvyCube() {
        setUpCurvyCubeVertexData();
    }

    CurvyCube(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shiny)
        : ambient(amb), diffuse(diff), specular(spec), shininess(shiny) {
        setUpCurvyCubeVertexData();
    }

    CurvyCube(unsigned int dMap, unsigned int sMap, float shiny, float textureXmin, float textureYmin, float textureXmax, float textureYmax)
        : diffuseMap(dMap), specularMap(sMap), shininess(shiny), TXmin(textureXmin), TYmin(textureYmin), TXmax(textureXmax), TYmax(textureYmax) {
        setUpCurvyCubeVertexData();
    }

    // Destructor
    ~CurvyCube() {
        glDeleteVertexArrays(1, &curvyCubeVAO);
        glDeleteBuffers(1, &curvyCubeVBO);
        glDeleteBuffers(1, &curvyCubeEBO);
    }

    void draw(Shader& shader, glm::mat4 model = glm::mat4(1.0f)) {
        shader.use();

        shader.setInt("material.diffuse", 0);
        shader.setInt("material.specular", 1);
        shader.setFloat("material.shininess", shininess);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        shader.setMat4("model", model);

        glBindVertexArray(curvyCubeVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

private:
    unsigned int curvyCubeVAO, curvyCubeVBO, curvyCubeEBO;

    void setUpCurvyCubeVertexData() {
        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        int subdivisions = 10; // Number of subdivisions per edge
        float step = 1.0f / subdivisions;

        // Generate vertices
        for (int x = 0; x <= subdivisions; ++x) {
            for (int y = 0; y <= subdivisions; ++y) {
                for (int z = 0; z <= subdivisions; ++z) {
                    float fx = x * step - 0.5f;
                    float fy = y * step - 0.5f;
                    float fz = z * step - 0.5f;

                    // Normalize to create curvature
                    glm::vec3 position = glm::normalize(glm::vec3(fx, fy, fz)) * 0.5f;
                    glm::vec3 normal = glm::normalize(position);

                    // Texture coordinates (simplified)
                    float u = (position.x + 0.5f);
                    float v = (position.y + 0.5f);

                    // Add to vertex data
                    vertices.push_back(position.x);
                    vertices.push_back(position.y);
                    vertices.push_back(position.z);
                    vertices.push_back(normal.x);
                    vertices.push_back(normal.y);
                    vertices.push_back(normal.z);
                    vertices.push_back(u);
                    vertices.push_back(v);
                }
            }
        }

        // Generate indices for triangles
        for (int x = 0; x < subdivisions; ++x) {
            for (int y = 0; y < subdivisions; ++y) {
                for (int z = 0; z < subdivisions; ++z) {
                    unsigned int base = x * (subdivisions + 1) * (subdivisions + 1) + y * (subdivisions + 1) + z;

                    indices.push_back(base);
                    indices.push_back(base + 1);
                    indices.push_back(base + (subdivisions + 1));

                    indices.push_back(base + 1);
                    indices.push_back(base + (subdivisions + 1) + 1);
                    indices.push_back(base + (subdivisions + 1));
                }
            }
        }

        // VAO, VBO, EBO setup
        glGenVertexArrays(1, &curvyCubeVAO);
        glGenBuffers(1, &curvyCubeVBO);
        glGenBuffers(1, &curvyCubeEBO);

        glBindVertexArray(curvyCubeVAO);

        glBindBuffer(GL_ARRAY_BUFFER, curvyCubeVBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, curvyCubeEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        // Vertex attributes
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
    }
};

#endif

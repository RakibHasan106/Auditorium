#ifndef FRACTAL_TREE_H
#define FRACTAL_TREE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include "Shader.h"

class FractalTree {
public:
    glm::vec3 branchColor; // Color of the branches
    glm::vec3 leafColor;   // Color of the leaves
    float branchWidth;     // Width of the branches

    FractalTree(float branchLength = 1.0f, float branchAngle = 30.0f, int recursionDepth = 10,
        glm::vec3 branchColor = glm::vec3(0.22f, 1.0f, 0.078f), float branchWidth = 5.0f,
        glm::vec3 leafColor = glm::vec3(0.0f, 1.0f, 0.0f))
    {
        this->branchLength = branchLength;
        this->branchAngle = branchAngle;
        this->recursionDepth = recursionDepth;
        this->branchColor = branchColor;
        this->leafColor = leafColor;
        this->branchWidth = branchWidth;

        buildTree();

        // Generate VAO and VBO for rendering
        glGenVertexArrays(1, &treeVAO);
        glBindVertexArray(treeVAO);

        glGenBuffers(1, &treeVBO);
        glBindBuffer(GL_ARRAY_BUFFER, treeVBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        // Enable vertex attribute for position
        glEnableVertexAttribArray(0); // Position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // Unbind VAO and VBO
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Leaves VBO
        glGenVertexArrays(1, &leafVAO);
        glBindVertexArray(leafVAO);

        glGenBuffers(1, &leafVBO);
        glBindBuffer(GL_ARRAY_BUFFER, leafVBO);
        glBufferData(GL_ARRAY_BUFFER, leafVertices.size() * sizeof(float), leafVertices.data(), GL_STATIC_DRAW);

        // Enable vertex attribute for position
        glEnableVertexAttribArray(0); // Position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    ~FractalTree() {
        glDeleteVertexArrays(1, &treeVAO);
        glDeleteBuffers(1, &treeVBO);
        glDeleteVertexArrays(1, &leafVAO);
        glDeleteBuffers(1, &leafVBO);
    }

    void drawTree(Shader& shader, glm::mat4 model) const {
        shader.use();

        // Draw branches
        shader.setVec3("color", branchColor);
        shader.setMat4("model", model);
        glLineWidth(branchWidth);
        glBindVertexArray(treeVAO);
        glDrawArrays(GL_LINES, 0, vertices.size() / 3);
        glBindVertexArray(0);
        glLineWidth(1.0f);

        // Draw leaves
        shader.setVec3("color", leafColor);
        glPointSize(3.0f); // Set size of leaf points
        glBindVertexArray(leafVAO);
        glDrawArrays(GL_POINTS, 0, leafVertices.size() / 3);
        glBindVertexArray(0);
    }

private:
    unsigned int treeVAO, treeVBO, leafVAO, leafVBO;
    float branchLength;    // Length of the branches
    float branchAngle;     // Angle between branches
    int recursionDepth;    // Maximum depth of recursion
    std::vector<float> vertices;    // Stores the tree's vertices
    std::vector<float> leafVertices; // Stores the leaf vertices

    void buildTree() {
        glm::vec3 start(0.0f, 0.0f, 0.0f); // Start at origin
        glm::vec3 direction(0.0f, branchLength, 0.0f); // Initial upward direction
        generateBranches(start, direction, recursionDepth);
    }

    void generateBranches(const glm::vec3& start, const glm::vec3& direction, int depth) {
        if (depth == 0) {
            generateLeaves(start); // Generate leaves at branch tips
            return;
        }

        // Compute the end point of the branch
        glm::vec3 end = start + direction;

        // Add the branch to the vertices list
        vertices.push_back(start.x);
        vertices.push_back(start.y);
        vertices.push_back(start.z);
        vertices.push_back(end.x);
        vertices.push_back(end.y);
        vertices.push_back(end.z);

        // Compute new branch directions
        float angleRadians = glm::radians(branchAngle);
        glm::mat4 rotationLeft = glm::rotate(glm::mat4(1.0f), angleRadians, glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 rotationRight = glm::rotate(glm::mat4(1.0f), -angleRadians, glm::vec3(0.0f, 0.0f, 1.0f));

        glm::vec3 leftDirection = glm::vec3(rotationLeft * glm::vec4(direction * 0.7f, 0.0f));
        glm::vec3 rightDirection = glm::vec3(rotationRight * glm::vec4(direction * 0.7f, 0.0f));

        // Recursively generate branches
        generateBranches(end, leftDirection, depth - 1);
        generateBranches(end, rightDirection, depth - 1);
    }

    void generateLeaves(const glm::vec3& position) {
        // Add a cluster of points around the given position
        for (int i = 0; i < 50; ++i) { // Generate 50 points per leaf cluster
            float offsetX = ((rand() % 100) / 100.0f - 0.5f) * 0.5f;
            float offsetY = ((rand() % 100) / 100.0f - 0.5f) * 0.5f;
            float offsetZ = ((rand() % 100) / 100.0f - 0.5f) * 0.5f;

            leafVertices.push_back(position.x + offsetX);
            leafVertices.push_back(position.y + offsetY);
            leafVertices.push_back(position.z + offsetZ);
        }
    }
};

#endif // FRACTAL_TREE_H

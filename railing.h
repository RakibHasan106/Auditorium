#ifndef RAILING_H
#define RAILING_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Cube.h"

class Railing {
public:
    glm::vec3 postColor;   // Color of the vertical posts
    glm::vec3 barColor;    // Color of the horizontal bars

    Railing(float postHeight = 0.7f, float postWidth = 0.1f, float postSpacing = 0.69f, int numPosts = 19,
        float barWidth = 0.08f, float barHeight = 0.7f, int numBars = 2, float barSpacing = 0.1f,
        glm::vec3 postColor = glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3 barColor = glm::vec3(0.3f, 0.3f, 0.3f))
    {
        this->postHeight = postHeight;
        this->postWidth = postWidth;
        this->postSpacing = postSpacing;
        this->numPosts = numPosts;

        this->barWidth = barWidth;
        this->barHeight = barHeight;
        this->numBars = numBars;
        this->barSpacing = barSpacing;

        this->postColor = postColor;
        this->barColor = barColor;
    }

    void drawRailing(Shader& shader, glm::mat4 model) const {
        // Draw vertical posts
        Cube c = Cube();
        for (int i = 0; i < numPosts; ++i) {
            float z = i * postSpacing;
            glm::mat4 postModel = glm::translate(model, glm::vec3(0.0f, postHeight / 2.0f, z));
            postModel = glm::scale(postModel, glm::vec3(postWidth, postHeight, postWidth));
            shader.use();
            shader.setVec3("color", postColor);
            c.drawCube(shader, postModel);
        }

        // Draw horizontal bars
        for (int i = 0; i < numBars; ++i) {
            float y = barHeight + i * barSpacing;
            glm::mat4 barModel = glm::translate(model, glm::vec3(0.0f, y, (numPosts - 1) * postSpacing / 2.0f));
            barModel = glm::scale(barModel, glm::vec3(barWidth, barWidth, (numPosts - 1) * postSpacing));
            shader.use();
            shader.setVec3("color", barColor);
            c.drawCube(shader, barModel);
        }
    }

private:
    float postHeight;       // Height of the vertical posts
    float postWidth;        // Width of the posts
    float postSpacing;      // Spacing between vertical posts
    int numPosts;           // Number of vertical posts

    float barWidth;         // Width of the bars
    float barHeight;        // Initial height of the horizontal bars
    int numBars;            // Number of horizontal bars
    float barSpacing;       // Spacing between horizontal bars
};

#endif // RAILING_H

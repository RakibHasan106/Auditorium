#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "basic_camera.h"
#include "pointLight.h"
#include "sphere.h"
#include "stb_image.h"
#include "cube.h"
#include "fractal.h"
#include "railing.h"
#include "BezierCurve.h"

#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model, float r, float g, float b, float shininess);
void axis(unsigned int& cubeVAO, Shader& lightingShader);
void floor(unsigned int& cubeVAO, Shader& lightingShader);
void chairs(unsigned int& cubeVAO, Shader& lightingShader);
void frontWall(unsigned int& cubeVAO, Shader& lightingShader);
void rightWall(unsigned int& cubeVAO, Shader& lightingShader);
void ambienton_off(Shader& lightingShader);
void diffuse_on_off(Shader& lightingShader);
void specular_on_off(Shader& lightingShader);
void walls(unsigned int& cubeVAO, Shader& lightingShader);
void stairs(unsigned int& cubeVAO, Shader& lightingShader);
unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax);

float left = -5.0f;
float right = 5.0f;
float bottom = -5.0f;
float top = 5.0f;
float near = 0.1f; 
float far = 100.0f;


glm::mat4 myProjection(float left, float right, float bottom, float top, float near, float far) {
    glm::mat4 result(0.0f); // Initialize to a zero matrix

    // Fill the perspective projection matrix
    result[0][0] = (2.0f * near) / (right - left);  // Scale x
    result[1][1] = (2.0f * near) / (top - bottom);  // Scale y
    result[2][0] = (right + left) / (right - left); // Translate x
    result[2][1] = (top + bottom) / (top - bottom); // Translate y
    result[2][2] = -(far + near) / (far - near);    // Scale z
    result[2][3] = -1.0f;                           // Perspective division
    result[3][2] = -(2.0f * far * near) / (far - near); // Translate z

    return result;
}

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Time management
double lastKeyPressTime = 0.0;
const double keyPressDelay = 0.2; // delay in seconds

// modelling transform
float rotateAngle_X = 0.0;
float rotateAngle_Y = 0.0;
float rotateAngle_Z = 0.0;
float rotateAxis_X = 0.0;
float rotateAxis_Y = 0.0;
float rotateAxis_Z = 1.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float translate_Z = 0.0;
float scale_X = 1.0;
float scale_Y = 1.0;
float scale_Z = 1.0;

// camera
Camera camera(glm::vec3(0.0f, 1.1f, -5.2f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float eyeX = 0.0, eyeY = 1.0, eyeZ = 3.0;
float lookAtX = 0.0, lookAtY = 0.0, lookAtZ = 0.0;
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);
BasicCamera basic_camera(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, V);


// positions of the point lights
glm::vec3 pointLightPositions[] = {
    glm::vec3(4.50f,  2.50f,  1.5f),
    glm::vec3(4.50f,  2.50f,  -1.5f)
    //glm::vec3(-1.5f,  1.5f,  0.0f),
    //glm::vec3(-1.5f,  -1.5f,  0.0f)
};
PointLight pointlight1(

    pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    1       // light number
);
PointLight pointlight2(

    pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    2       // light number
);
//PointLight pointlight3(
//
//    pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z,  // position
//    0.05f, 0.05f, 0.05f,     // ambient
//    0.8f, 0.8f, 0.8f,     // diffuse
//    1.0f, 1.0f, 1.0f,        // specular
//    1.0f,   //k_c
//    0.09f,  //k_l
//    0.032f, //k_q
//    3       // light number
//);
//PointLight pointlight4(
//
//    pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z,  // position
//    0.05f, 0.05f, 0.05f,     // ambient
//    0.8f, 0.8f, 0.8f,     // diffuse
//    1.0f, 1.0f, 1.0f,        // specular
//    1.0f,   //k_c
//    0.09f,  //k_l
//    0.032f, //k_q
//    4       // light number
//);


// light settings
bool pointLightOn = true;
bool directionalLightOn = true;
bool SpotLightOn = true;
bool AmbientON = true;
bool DiffusionON = true;
bool SpecularON = true;
bool ambientToggle = true;
bool diffuseToggle = true;
bool specularToggle = true;


// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;

int main()
{

    float fov = glm::radians(45.0f);               // Field of view in radians
    float aspect = 16.0f / 9.0f;                  // Aspect ratio (e.g., 1920x1080 screen)
    float near = 0.1f;                            // Near clipping plane
    float far = 100.0f;                           // Far clipping plane

    // Calculate boundaries for the near clipping plane
    float top = tan(fov / 2.0f) * near;           // Top boundary
    float bottom = -top;                          // Bottom boundary
    float right = top * aspect;                   // Right boundary
    float left = -right;


    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Auditorium", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    //glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    Torus torus(2.0f, 0.6f, 50, 30); // Torus with radius = 1.0, tube radius = 0.4
    /*Shader lightingShader("vertex_shader.glsl", "fragment_shader.glsl");*/
    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShader("vertexShaderForPhongShading.vs", "fragmentShaderForPhongShading.fs");
    Shader lightingShaderWithTexture("vertexShaderForPhongShadingWithTexture.vs", "fragmentShaderForPhongShadingWithTexture.fs");
    //Shader lightingShader("vertexShaderForGouraudShading.vs", "fragmentShaderForGouraudShading.fs");
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    string diffuseMapPath = "container2.png";
    string specularMapPath = "container2_specular.png";
    string laughEmoPath = "icon.png";
    string floorTexturePath = "floor_texture.jpg";
    string curveTexturePath = "curve_texture.jpg";

    unsigned int laughEmoji = loadTexture(laughEmoPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    //unsigned int laughEmojiv2 = loadTexture(laughEmoPath.c_str(), GL_REPEAT, GL_MIRRORED_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    
    unsigned int floor_texture = loadTexture(floorTexturePath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    unsigned int curveTexture = loadTexture(curveTexturePath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    
    Cube floorCube = Cube(floor_texture, floor_texture, 32.0f, 0.0f, 0.0f, 2.0f, 2.0f);



    Cube cube = Cube(laughEmoji, laughEmoji, 32.0f, 0.0f, 0.0f, 2.0f, 2.0f);
    SphereTex spheretex = SphereTex();

    //fractal for tree
    FractalTree tree = FractalTree();

    //Railing on the second floor
    Railing railing = Railing();

    

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    float curve_points[] = {
-0.0050, 1.7348, 4.9635,
-0.2350, 1.7201, 4.9666,
-0.4850, 1.6467, 4.9821,
-0.6100, 1.5391, 5.0049,
-0.7350, 1.3924, 5.0359,
-0.7400, 1.2945, 5.0566,
-0.7500, 1.1820, 5.0804,
-0.7300, 1.0695, 5.1042,
-0.7600, 1.0059, 5.1177,
-0.8250, 0.9326, 5.1332,
-0.9550, 0.9032, 5.1394,
-1.1000, 0.8787, 5.1446,
-1.2400, 0.9423, 5.1311,
-1.3500, 1.0157, 5.1156,
    };

    //Bezier Curve
    BezierCurve bezierCurve = BezierCurve(curve_points,16,curveTexture);

    float cube_vertices[] = {
        // positions      // normals
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,

        1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

        0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

        0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f
    };
    unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        4, 5, 7,
        7, 6, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20
    };


    unsigned int cubeVAO, cubeVBO, cubeEBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &cubeEBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);


    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // vertex normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    /*Cone cone = Cone();*/

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);



    //ourShader.use();
    //lightingShader.use();

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);

        lightingShaderWithTexture.use();
        lightingShaderWithTexture.setVec3("viewPos", camera.Position);

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection_texture = myProjection(left, right, bottom, top, near, far);
        lightingShaderWithTexture.setMat4("projection", projection_texture);


        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        //glm::mat4 view = basic_camera.createViewMatrix();
        lightingShaderWithTexture.setMat4("view", view);



        // point light 1
        pointlight1.setUpPointLight(lightingShader);
        // point light 2
        pointlight2.setUpPointLight(lightingShader);
        // point light 3
        //pointlight3.setUpPointLight(lightingShader);
        // point light 4
        //pointlight4.setUpPointLight(lightingShader);

        // activate shader
        lightingShader.use();

        lightingShader.setVec3("directionalLight.direction", 0.5f, -3.0f, -3.0f);
        if (AmbientON) {
            lightingShader.setVec3("directionalLight.ambient", 0.2f, 0.2f, 0.2f);
            lightingShader.setVec3("spotLight.ambient", 0.2f, 0.2f, 0.2f);
        }
        if (DiffusionON) {
            lightingShader.setVec3("directionalLight.diffuse", 0.8f, 0.8f, 0.8f);
            lightingShader.setVec3("spotLight.diffuse", 0.8f, 0.8f, 0.8f);
        }
        if (SpecularON) {

            lightingShader.setVec3("directionalLight.specular", 1.0f, 1.0f, 1.0f);
            lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        }



        lightingShader.setBool("directionalLightON", directionalLightOn);
        lightingShader.setBool("SpotLightON", SpotLightOn);

        lightingShader.setVec3("spotLight.direction", 0.0f, -1.0f, 0.0f);
        lightingShader.setVec3("spotLight.position", 3.5f, 3.0f, 0.0f);


        lightingShader.setFloat("spotLight.k_c", 1.0f);
        lightingShader.setFloat("spotLight.k_l", 0.09f);
        lightingShader.setFloat("spotLight.k_q", 0.032f);
        lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(35.5f)));
        lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(40.5f)));

        // pass projection matrix to shader (note that in this case it could change every frame)
       // glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);

        //glm::mat4 projection = myPerspective(
        //    glm::radians(camera.Zoom),         // fov: field of view in radians
        //    (float)SCR_WIDTH / (float)SCR_HEIGHT, // aspect: aspect ratio
        //    0.1f,                              // near: near clipping plane
        //    100.0f                             // far: far clipping plane
        //);

        glm::mat4 projection = myProjection(left, right, bottom, top, near, far);

        lightingShader.setMat4("projection", projection);

        // camera/view transformation
        //glm::mat4 view = basic_camera.createViewMatrix();
        lightingShader.setMat4("view", view);

        // Modelling Transformation
        glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);

        lightingShaderWithTexture.use();
        pointlight1.setUpPointLight(lightingShaderWithTexture);
        pointlight2.setUpPointLight(lightingShaderWithTexture);

        glm::mat4 modelMatrixForContainer = glm::mat4(1.0f);
        //modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(4.87f, 2.5f, 0.1f)) * glm::scale(identityMatrix, glm::vec3(0.0f,1.0f,1.5f));
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(2.0f, 2.5f, 0.1f));
        cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        
        //1st floor
        glm::mat4 modelMatrixForFloorContainer = glm::mat4(1.0f);
        modelMatrixForFloorContainer = ((glm::scale(identityMatrix, glm::vec3(30.0f, 0.2f, 30.0f))) *
            (glm::translate(identityMatrix, glm::vec3(-0.23f, -4.5f, 0.0f))));
        floorCube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForFloorContainer);

        //2nd floor
        glm::mat4 translate2ndfloor = glm::mat4(1.0f);
        //glm::mat4 translate2 = glm::mat4(1.0f);
        glm::mat4 scale2ndfloor = glm::mat4(1.0f);

        glm::mat4 modelMatrixFor2ndFloorContainer = glm::mat4(1.0f);

        translate2ndfloor = glm::scale(identityMatrix, glm::vec3(-8.7f, 0.2f, 25.0f));
        scale2ndfloor = glm::translate(identityMatrix, glm::vec3(2.0f, 15.0f, -0.1f)) ;

        modelMatrixFor2ndFloorContainer = translate2ndfloor * scale2ndfloor;
        floorCube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixFor2ndFloorContainer);

        //draw Tree using fractal
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f,0.0f,0.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
        glm::mat4 modelMatrixForTree = translateMatrix * scaleMatrix;
        tree.drawTree(lightingShader, modelMatrixForTree);

        //drawing railing on the second floor
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-13.5f, 3.0f, -14.9f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
        //rotateMatrix = glm::rotate()
        glm::mat4 modelMatrixForRailing = translateMatrix * scaleMatrix;
        railing.drawRailing(lightingShader, modelMatrixForRailing);


        //roof
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
        glm::mat4 modelMatrixForRoof = translateMatrix * scaleMatrix;
        bezierCurve.drawBezierCurve(lightingShader, modelMatrixForRoof);

        //spheretex.drawSphere(lightingShaderWithTexture, laughEmoji, modelMatrixForContainer);

        //glBindVertexArray(cubeVAO);
        //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        //glDrawArrays(GL_TRIANGLES, 0, 36);

        //bed(cubeVAO, lightingShader, model);
        //draw floor
        //floor(cubeVAO, lightingShader);
        axis(cubeVAO, lightingShader);
        frontWall(cubeVAO, lightingShader);
        chairs(cubeVAO, lightingShader);
        walls(cubeVAO, lightingShader);
        stairs(cubeVAO, lightingShader);


        //second floor
        //floor(cubeVAO, lightingShader);
        

        /*rightWall(cubeVAO, lightingShader);*/

        // also draw the lamp object(s)
        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // we now draw as many light bulbs as we have point lights.
        glBindVertexArray(lightCubeVAO);
        for (unsigned int i = 0; i < 2; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
            ourShader.setMat4("model", model);
            ourShader.setVec3("color", glm::vec3(0.8f, 0.8f, 0.8f));
            /*cone.drawCone(lightingShader, model);*/
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
        {
            ambienton_off(lightingShader);
        }
        if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
        {
            diffuse_on_off(lightingShader);
        }
        if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
        {
            specular_on_off(lightingShader);
        }


        //Torus code

        /*glm::mat4 model = glm::mat4(1.0f);*/
        //model = glm::translate(model, glm::vec3(-10.0f, -8.19f, -0.2f)); // Adjust position
        //model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // 90 degrees about X-axis
        //model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f)); // 45 degrees about Z-axis
        //model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // 45 degrees about Z-axis
        //torus.drawTorus(lightingShader, model);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &cubeEBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f), float r = 1.0f, float g = 1.0f, float b = 1.0f, float shininess = 32.0f)
{
    lightingShader.use();

    lightingShader.setVec3("material.ambient", glm::vec3(r, g, b));
    lightingShader.setVec3("material.diffuse", glm::vec3(r, g, b));
    lightingShader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    lightingShader.setFloat("material.shininess", shininess);

    lightingShader.setMat4("model", model);

    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void floor(unsigned int& cubeVAO, Shader& lightingShader)
{
    //second floor
    lightingShader.use();
    //base
    glm::mat4 identityMatrix = glm::mat4(1.0f);
    glm::mat4 translate = glm::mat4(1.0f);
    //glm::mat4 translate2 = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);
    scale = glm::scale(identityMatrix, glm::vec3(-10.0, 0.2, 10.0));
    translate = glm::translate(identityMatrix, glm::vec3(-10.0, 4.0, -5.0));
    glm::mat4 model = translate * scale;
    drawCube(cubeVAO, lightingShader, model, 1.0, 1.0, 1.0, 32.0);

    /*scale = glm::scale(identityMatrix, glm::vec3(0.1, 2.0, 0.1));
    translate = glm::translate(identityMatrix, glm::vec3(-5.0, -0.8, -5.0));
    model = translate * scale;
    drawCube(cubeVAO, lightingShader, model, 0.1, 0.1, 0.1, 32.0);*/

    //wall
    /*scale = glm::scale(identityMatrix, glm::vec3(-5.0, 5.0, 0.1));
    translate = glm::translate(identityMatrix, glm::vec3(4.9, -0.8, 2.0));
    model = translate * scale;
    drawCube(cubeVAO, lightingShader, model, 0.5, 0.5, 0.5, 32.0);*/
}
void chairs(unsigned int& cubeVAO, Shader& lightingShader) {
    lightingShader.use();
    //base
    glm::mat4 identityMatrix = glm::mat4(1.0f);
    glm::mat4 translate = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);
    glm::mat4 rotation = glm::mat4(1.0f);
    glm::mat4 model = translate * scale;

    float chairdistanceX = 1.5f;
    float chairdistanceY = 2.5f;

    //left columns
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            scale = glm::scale(identityMatrix, glm::vec3(0.8, 0.5, 1.0));
            translate = glm::translate(identityMatrix, glm::vec3(-2.5 - (chairdistanceY * i), -0.8, -14.0 + (chairdistanceX * j)));
            model = translate * scale;
            drawCube(cubeVAO, lightingShader, model, 0.0f, 0.0f, 1.0f, 32.0f); // Blue seat

            //Back Rest
            scale = glm::scale(identityMatrix, glm::vec3(0.3, 1.0, 1.0));
            translate = glm::translate(identityMatrix, glm::vec3(-2.5 - (chairdistanceY * i), -0.8, -14.0 + (chairdistanceX * j)));
            model = translate * scale;
            drawCube(cubeVAO, lightingShader, model, 0.0f, 0.0f, 1.0f, 32.0f);
        }

    }


    //mid columns
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            scale = glm::scale(identityMatrix, glm::vec3(0.8, 0.5, 1.0));
            translate = glm::translate(identityMatrix, glm::vec3(-2.5 - (chairdistanceY * i), -0.8, -4.0 + (chairdistanceX * j)));
            model = translate * scale;
            drawCube(cubeVAO, lightingShader, model, 0.0f, 0.0f, 1.0f, 32.0f); // Blue seat

            //Back Rest
            scale = glm::scale(identityMatrix, glm::vec3(0.3, 1.0, 1.0));
            translate = glm::translate(identityMatrix, glm::vec3(-2.5 - (chairdistanceY * i), -0.8, -4.0 + (chairdistanceX * j)));
            model = translate * scale;
            drawCube(cubeVAO, lightingShader, model, 0.0f, 0.0f, 1.0f, 32.0f);
        }
        
    }

    //right columns
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            scale = glm::scale(identityMatrix, glm::vec3(0.8, 0.5, 1.0));
            translate = glm::translate(identityMatrix, glm::vec3(-2.5 - (chairdistanceY * i), -0.8, 6.0 + (chairdistanceX * j)));
            model = translate * scale;
            drawCube(cubeVAO, lightingShader, model, 0.0f, 0.0f, 1.0f, 32.0f); // Blue seat

            //Back Rest
            scale = glm::scale(identityMatrix, glm::vec3(0.3, 1.0, 1.0));
            translate = glm::translate(identityMatrix, glm::vec3(-2.5 - (chairdistanceY * i), -0.8, 6.0 + (chairdistanceX * j)));
            model = translate * scale;
            drawCube(cubeVAO, lightingShader, model, 0.0f, 0.0f, 1.0f, 32.0f);
        }

    }

    //chairs at the second floor
    //left column
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            scale = glm::scale(identityMatrix, glm::vec3(0.8, 0.5, 1.0));
            translate = glm::translate(identityMatrix, glm::vec3(-15 - (chairdistanceY * i), 3.0, -14.0 + (chairdistanceX * j)));
            model = translate * scale;
            drawCube(cubeVAO, lightingShader, model, 0.0f, 0.0f, 1.0f, 32.0f); // Blue seat

            //Back Rest
            scale = glm::scale(identityMatrix, glm::vec3(0.3, 1.0, 1.0));
            translate = glm::translate(identityMatrix, glm::vec3(-15 - (chairdistanceY * i), 3.0, -14.0 + (chairdistanceX * j)));
            model = translate * scale;
            drawCube(cubeVAO, lightingShader, model, 0.0f, 0.0f, 1.0f, 32.0f);
        }

    }

    //mid column
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 6; j++) {
            scale = glm::scale(identityMatrix, glm::vec3(0.8, 0.5, 1.0));
            translate = glm::translate(identityMatrix, glm::vec3(-15 - (chairdistanceY * i), 3.0, -7.0 + (chairdistanceX * j)));
            model = translate * scale;
            drawCube(cubeVAO, lightingShader, model, 0.0f, 0.0f, 1.0f, 32.0f); // Blue seat

            //Back Rest
            scale = glm::scale(identityMatrix, glm::vec3(0.3, 1.0, 1.0));
            translate = glm::translate(identityMatrix, glm::vec3(-15 - (chairdistanceY * i), 3.0, -7.0 + (chairdistanceX * j)));
            model = translate * scale;
            drawCube(cubeVAO, lightingShader, model, 0.0f, 0.0f, 1.0f, 32.0f);
        }

    }

    //right column
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            scale = glm::scale(identityMatrix, glm::vec3(0.8, 0.5, 1.0));
            translate = glm::translate(identityMatrix, glm::vec3(-15 - (chairdistanceY * i), 3.0, 4.0 + (chairdistanceX * j)));
            model = translate * scale;
            drawCube(cubeVAO, lightingShader, model, 0.0f, 0.0f, 1.0f, 32.0f); // Blue seat

            //Back Rest
            scale = glm::scale(identityMatrix, glm::vec3(0.3, 1.0, 1.0));
            translate = glm::translate(identityMatrix, glm::vec3(-15 - (chairdistanceY * i), 3.0, 4.0 + (chairdistanceX * j)));
            model = translate * scale;
            drawCube(cubeVAO, lightingShader, model, 0.0f, 0.0f, 1.0f, 32.0f);
        }

    }
     
    
    //scale = glm::scale(identityMatrix, glm::vec3( 0.8, 0.5, 1.0));
    //translate = glm::translate(identityMatrix, glm::vec3(-2.5, -0.8, -4.5));
    //model = translate * scale;
    //drawCube(cubeVAO, lightingShader, model, 0.0f, 0.0f, 1.0f, 32.0f); // Blue seat
    //
    ////Back Rest
    //scale = glm::scale(identityMatrix, glm::vec3(0.3, 1.0, 1.0));
    //translate = glm::translate(identityMatrix, glm::vec3(-2.5, -0.8, -4.5));
    //model = translate * scale;
    //drawCube(cubeVAO, lightingShader, model, 0.0f, 0.0f, 1.0f, 32.0f);









}


void frontWall(unsigned int& cubeVAO, Shader& lightingShader)
{
    lightingShader.use();
    //base
    glm::mat4 identityMatrix = glm::mat4(1.0f);
    glm::mat4 translate = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);
    glm::mat4 rotation = glm::mat4(1.0f);
    glm::mat4 model = translate * scale;

    //wall
    scale = glm::scale(identityMatrix, glm::vec3(0.1, 10.0, 30.0));
    translate = glm::translate(identityMatrix, glm::vec3(4.9, -0.8, -15.0));
    model = translate * scale;
    drawCube(cubeVAO, lightingShader, model, 0.5, 0.5, 0.5, 32.0);

    //podium
    scale = glm::scale(identityMatrix, glm::vec3(-1.0, 1.8, 1.0));
    translate = glm::translate(identityMatrix, glm::vec3(3.0, 0.0, -3.0));
    model = translate * scale;
    drawCube(cubeVAO, lightingShader, model, 0.431, 0.149, 0.055, 1.0);

    //stage code upper

    scale = glm::scale(identityMatrix, glm::vec3(-4.0, 0.1, 8.0));
    translate = glm::translate(identityMatrix, glm::vec3(5.0, -0.1, -4.0));
    model = translate * scale;
    drawCube(cubeVAO, lightingShader, model, 0.624, 0.396, 0.188, 1.0);

    //stage code lower
    scale = glm::scale(identityMatrix, glm::vec3(-4.0, 0.7, 8.0));
    translate = glm::translate(identityMatrix, glm::vec3(5.0, -0.8, -4.0));
    model = translate * scale;
    drawCube(cubeVAO, lightingShader, model, 0.431, 0.149, 0.055, 1.0);


    //stage stairs
    scale = glm::scale(identityMatrix, glm::vec3(-0.6, 0.6, 0.5));
    translate = glm::translate(identityMatrix, glm::vec3(1.7, -0.8, -4.5));
    model = translate * scale;
    drawCube(cubeVAO, lightingShader, model, 0.431, 0.149, 0.055, 32.0);

    //stage stairs
    scale = glm::scale(identityMatrix, glm::vec3(-0.6, 0.3, 0.5));
    translate = glm::translate(identityMatrix, glm::vec3(1.7, -0.8, -4.9));
    model = translate * scale;
    drawCube(cubeVAO, lightingShader, model, 0.431, 0.149, 0.055, 32.0);


    /*scale = glm::scale(identityMatrix, glm::vec3(-1.0, 0.8, 10.0));
    translate = glm::translate(identityMatrix, glm::vec3(1.0, -0.8, -5.0));
    model = translate * scale;
    drawCube(cubeVAO, lightingShader, model, 0.112, 0.167, 0.231, 32.0);*/

    /*scale = glm::scale(identityMatrix, glm::vec3(-1.0, 0.4, 10.0));
    translate = glm::translate(identityMatrix, glm::vec3(0.0, -0.8, -5.0));
    model = translate * scale;
    drawCube(cubeVAO, lightingShader, model, 0.112, 0.167, 0.231, 32.0);*/

    ////lamp holder
    //scale = glm::scale(identityMatrix, glm::vec3(0.6, 0.05, 0.05));
    //translate = glm::translate(identityMatrix, glm::vec3(4.4f, 2.9f, 1.475f));
    //model = translate * scale;
    //drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0, 32.0);

    //translate = glm::translate(identityMatrix, glm::vec3(4.4f, 2.9f, -1.525f));
    //model = translate * scale;
    //drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0, 32.0);

}

void rightWall(unsigned int& cubeVAO, Shader& lightingShader)
{
    lightingShader.use();
    //base
   /* glm::mat4 identityMatrix = glm::mat4(1.0f);
    glm::mat4 translate = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);
    glm::mat4 rotation = glm::mat4(1.0f);
    glm::mat4 model = translate * scale;*/
    //wall
    /*scale = glm::scale(identityMatrix, glm::vec3(10.0, 5.0, 0.1));
    translate = glm::translate(identityMatrix, glm::vec3(-5.0, -0.8, 4.9));
    model = translate * scale;
    drawCube(cubeVAO, lightingShader, model, 0.5, 0.5, 0.5, 32.0);

    scale = glm::scale(identityMatrix, glm::vec3(1.7, 1.5, 0.7));
    translate = glm::translate(identityMatrix, glm::vec3(-3.85, -0.1, 4.0));
    model = translate * scale;
    drawCube(cubeVAO, lightingShader, model, 0.122, 0.361, 0.357, 32.0);*/


}
void walls(unsigned int& cubeVAO, Shader& lightingShader) {
    lightingShader.use();
    //base
    glm::mat4 identityMatrix = glm::mat4(1.0f);
    glm::mat4 translate = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);
    glm::mat4 rotation = glm::mat4(1.0f);
    glm::mat4 model = translate * scale;

    //left wall
    scale = glm::scale(identityMatrix, glm::vec3(-26.8, 10.0, 0.1));
    translate = glm::translate(identityMatrix, glm::vec3(4.9, -0.8, -15.0));
    model = translate * scale;
    drawCube(cubeVAO, lightingShader, model, 0.5, 0.5, 0.5, 32.0);

    //right wall
    scale = glm::scale(identityMatrix, glm::vec3(-26.8, 10.0, 0.1));
    translate = glm::translate(identityMatrix, glm::vec3(4.9, -0.8, 15.0));
    model = translate * scale;
    drawCube(cubeVAO, lightingShader, model, 0.5, 0.5, 0.5, 32.0);

    //back wall
    scale = glm::scale(identityMatrix, glm::vec3(0.1, 10.0, 30.0));
    translate = glm::translate(identityMatrix, glm::vec3(-22, -0.8, -15.0));
    model = translate * scale ;
    drawCube(cubeVAO, lightingShader, model, 0.5, 0.5, 0.5, 32.0);




}
void stairs(unsigned int& cubeVAO, Shader& lightingShader) {
    lightingShader.use();

    float stepWidth = 1.0f;
    float stepHeight = 0.5f;
    float stepDepth = 1.0f;
    int numSteps = 4;

    glm::mat4 identityMatrix = glm::mat4(1.0f);
    glm::mat4 translate = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);
    glm::mat4 rotation = glm::mat4(1.0f);
    glm::mat4 model = translate * scale;

    for (int i = 0; i < numSteps; i++) {

        scale = glm::scale(identityMatrix, glm::vec3(-2.0, 0.5 + (stepHeight * i), -1.0));
        translate = glm::translate(identityMatrix, glm::vec3(-18.0 , -0.9, 12.0 +(stepWidth * i)));
        model = translate * scale;
        drawCube(cubeVAO, lightingShader, model, 0.5, 0.5, 0.5, 32.0);
    }

    for (int i = 0; i < numSteps+1; i++) {

        scale = glm::scale(identityMatrix, glm::vec3(-2.0, 0.5 , -1.0));
        translate = glm::translate(identityMatrix, glm::vec3(-16.0, 0.6 + (stepHeight * i), 15.0 - (stepWidth * i)));
        model = translate * scale;
        drawCube(cubeVAO, lightingShader, model, 0.5, 0.5, 0.5, 32.0);
    }
    

    //for (int i = 0; i < numSteps; i++) {
    //    // Calculate position of the step
    //    float x = 0.0f;
    //    float y = i * stepHeight;
    //    float z = -i * stepDepth;

    //    // Transformation matrix for the step
    //    glm::mat4 step = glm::mat4(1.0f);
    //    step = glm::translate(step, glm::vec3(x, y, z));
    //    step = glm::scale(step, glm::vec3(stepWidth, stepHeight, stepDepth));

    //    // Draw the step
    //    drawCube(cubeVAO, lightingShader, step, 0.7f, 0.5f, 0.3f); // Brownish color
    //}
}

void axis(unsigned int& cubeVAO, Shader& lightingShader)
{
    lightingShader.use();
    //base
    glm::mat4 identityMatrix = glm::mat4(1.0f);
    glm::mat4 translate = glm::mat4(1.0f);
    //glm::mat4 translate2 = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);
    //x
    scale = glm::scale(identityMatrix, glm::vec3(2.0, 0.1, 0.1));
    translate = glm::translate(identityMatrix, glm::vec3(0.0, 0.0, 0.0));
    glm::mat4 model = translate * scale;
    drawCube(cubeVAO, lightingShader, model, 1.0, 0.0, 0.0, 32.0);
    //y
    scale = glm::scale(identityMatrix, glm::vec3(0.1, 2.0, 0.1));
    translate = glm::translate(identityMatrix, glm::vec3(0.0, 0.0, 0.0));
    model = translate * scale;
    drawCube(cubeVAO, lightingShader, model, 0.0, 1.0, 0.0, 32.0);
    //z
    scale = glm::scale(identityMatrix, glm::vec3(0.1, 0.1, 2.0));
    translate = glm::translate(identityMatrix, glm::vec3(0.0, 0.0, 0.0));
    model = translate * scale;
    drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 1.0, 32.0);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        if (rotateAxis_X) rotateAngle_X -= 0.1;
        else if (rotateAxis_Y) rotateAngle_Y -= 0.1;
        else rotateAngle_Z -= 0.1;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        camera.ProcessKeyboard(UP, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        camera.ProcessKeyboard(DOWN, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
        camera.ProcessKeyboard(Y_LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
        camera.ProcessKeyboard(Y_RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        camera.ProcessKeyboard(R_LEFT, deltaTime);

    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        camera.ProcessKeyboard(R_RIGHT, deltaTime);

    }
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        camera.ProcessKeyboard(P_UP, deltaTime);

    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        camera.ProcessKeyboard(P_DOWN, deltaTime);
    }
    //if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) translate_Y += 0.01;
    //if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) translate_Y -= 0.01;
    //if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) translate_X += 0.01;
    //if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) translate_X -= 0.01;
    //if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) translate_Z += 0.01;
    //if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) translate_Z -= 0.01;
    //if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) scale_X += 0.001;
    //if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) scale_X -= 0.001;
    //if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) scale_Y += 0.001;
    //if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) scale_Y -= 0.001;
    //if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) scale_Z += 0.001;
    //if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) scale_Z -= 0.001;

    //if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    //{
    //    rotateAngle_X += 0.1;
    //    rotateAxis_X = 1.0;
    //    rotateAxis_Y = 0.0;
    //    rotateAxis_Z = 0.0;
    //}
    //if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
    //{
    //    rotateAngle_Y += 0.1;
    //    rotateAxis_X = 0.0;
    //    rotateAxis_Y = 1.0;
    //    rotateAxis_Z = 0.0;
    //}
    //if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    //{
    //    rotateAngle_Z += 0.1;
    //    rotateAxis_X = 0.0;
    //    rotateAxis_Y = 0.0;
    //    rotateAxis_Z = 1.0;
    //}

    //if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
    //{
    //    eyeX += 2.5 * deltaTime;
    //    basic_camera.changeEye(eyeX, eyeY, eyeZ);
    //}
    //if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    //{
    //    eyeX -= 2.5 * deltaTime;
    //    basic_camera.changeEye(eyeX, eyeY, eyeZ);
    //}
    //if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
    //{
    //    eyeZ += 2.5 * deltaTime;
    //    basic_camera.changeEye(eyeX, eyeY, eyeZ);
    //}
    //if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
    //{
    //    eyeZ -= 2.5 * deltaTime;
    //    basic_camera.changeEye(eyeX, eyeY, eyeZ);
    //}
    //if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    //{
    //    eyeY += 2.5 * deltaTime;
    //    basic_camera.changeEye(eyeX, eyeY, eyeZ);
    //}
    //if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    //{
    //    eyeY -= 2.5 * deltaTime;
    //    basic_camera.changeEye(eyeX, eyeY, eyeZ);
    //}

}
void ambienton_off(Shader& lightingShader)
{
    double currentTime = glfwGetTime();
    if (currentTime - lastKeyPressTime < keyPressDelay) return;
    lightingShader.use();
    if (AmbientON)
    {
        pointlight1.turnAmbientOff();
        pointlight2.turnAmbientOff();
        lightingShader.setVec3("directionalLight.ambient", 0.0f, 0.0f, 0.0f);
        lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        AmbientON = !AmbientON;
        lastKeyPressTime = currentTime;
    }
    else
    {
        pointlight1.turnAmbientOn();
        pointlight2.turnAmbientOn();
        lightingShader.setVec3("directionalLight.ambient", 0.2f, 0.2f, 0.2f);
        lightingShader.setVec3("spotLight.ambient", 0.2f, 0.2f, 0.2f);
        AmbientON = !AmbientON;
        lastKeyPressTime = currentTime;
    }
}
void diffuse_on_off(Shader& lightingShader)
{
    double currentTime = glfwGetTime();
    if (currentTime - lastKeyPressTime < keyPressDelay) return;
    lightingShader.use();
    if (DiffusionON)
    {
        pointlight1.turnDiffuseOff();
        pointlight2.turnDiffuseOff();
        lightingShader.setVec3("directionalLight.diffuse", 0.0f, 0.0f, 0.0f);
        lightingShader.setVec3("spotLight.diffuse", 0.0f, 0.0f, 0.0f);
        DiffusionON = !DiffusionON;
        lastKeyPressTime = currentTime;
    }
    else
    {
        pointlight1.turnDiffuseOn();
        pointlight2.turnDiffuseOn();
        lightingShader.setVec3("directionalLight.diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader.setVec3("spotLight.diffuse", 0.8f, 0.8f, 0.8f);
        DiffusionON = !DiffusionON;
        lastKeyPressTime = currentTime;
    }
}
void specular_on_off(Shader& lightingShader)
{
    double currentTime = glfwGetTime();
    if (currentTime - lastKeyPressTime < keyPressDelay) return;
    lightingShader.use();
    if (SpecularON)
    {
        pointlight1.turnSpecularOff();
        pointlight2.turnSpecularOff();
        lightingShader.setVec3("directionalLight.specular", 0.0f, 0.0f, 0.0f);
        lightingShader.setVec3("spotLight.specular", 0.0f, 0.0f, 0.0f);
        SpecularON = !SpecularON;
        lastKeyPressTime = currentTime;
    }
    else
    {
        pointlight1.turnSpecularOn();
        pointlight2.turnSpecularOn();
        lightingShader.setVec3("directionalLight.specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        SpecularON = !SpecularON;
        lastKeyPressTime = currentTime;
    }
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    //if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    //{
    //    if (pointLightOn)
    //    {
    //        pointlight1.turnOff();
    //        pointlight2.turnOff();
    //        pointlight3.turnOff();
    //        pointlight4.turnOff();
    //        pointLightOn = !pointLightOn;
    //    }
    //    else
    //    {
    //        pointlight1.turnOn();
    //        pointlight2.turnOn();
    //        pointlight3.turnOn();
    //        pointlight4.turnOn();
    //        pointLightOn = !pointLightOn;
    //    }
    //}

    if (key == GLFW_KEY_2 && action == GLFW_PRESS)
    {
        if (pointLightOn)
        {
            pointlight1.turnOff();
            pointlight2.turnOff();
            //pointlight3.turnOff();
            //pointlight4.turnOff();
            pointLightOn = !pointLightOn;
        }
        else
        {
            pointlight1.turnOn();
            pointlight2.turnOn();
            //pointlight3.turnOn();
            //pointlight4.turnOn();
            pointLightOn = !pointLightOn;
        }
    }
    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        if (directionalLightOn)
        {
            directionalLightOn = !directionalLightOn;
        }
        else
        {
            directionalLightOn = !directionalLightOn;
        }
    }
    if (key == GLFW_KEY_3 && action == GLFW_PRESS)
    {
        if (SpotLightOn)
        {
            SpotLightOn = !SpotLightOn;
        }
        else
        {
            SpotLightOn = !SpotLightOn;
        }
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrappingModeS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrappingModeT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureFilteringModeMin);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureFilteringModeMax);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
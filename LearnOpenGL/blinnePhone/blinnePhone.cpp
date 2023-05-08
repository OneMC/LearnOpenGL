//
//  blinnePhone.cpp
//  LearnOpenGL
//
//  Created by 苗超 on 2023/4/30.
//

#include "blinnePhone.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

extern void loadTexture(char const * texturePath, unsigned int *texture, int colorType);

void BlinnPhongRender::setup() {
    setupPlane();
    // -----------------------------
    glEnable(GL_DEPTH_TEST); // 开启深度测试
    glEnable(GL_BLEND);      // 开启blend
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // 设置blend策略
    
    // build and compile shaders
    // -------------------------
    _shader = new Shader(ResPath("/code_resource/advanced_lighting/advanced_lighting_vertex.vs"), ResPath("/code_resource/advanced_lighting/advanced_lighting_fragment.fs"));
    _shader->use();
    _shader->setInt("texture1", 0);
    
    // load textures
    // -------------
    loadTexture(ResPath("/code_resource/advanced_lighting/wood.png"), &floorTexture,GL_RGB);
}

void BlinnPhongRender::setupPlane() {
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float planeVertices[] = {
        // positions            // normals         // texcoords
        10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
        -10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
        -10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
        
        10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
        -10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
        10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,  10.0f, 10.0f
    };
    
    // plane VAO
    unsigned int planeVBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glBindVertexArray(0);
}

void BlinnPhongRender::setupCamera() {
    CamerRenderA::setupCamera();
    glm::mat4 view = viewModel();
    glm::mat4 projection = projectionModel();
    _shader->use();
    _shader->setMat4("view", view);
    _shader->setMat4("projection", projection);
}

void BlinnPhongRender::render() {
    setupCamera();
    
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
    // set light uniforms
    _shader->setVec3("viewPos", _camera->Position);
    _shader->setVec3("lightPos", lightPos);
    _shader->setInt("blinn", blinn);
    
    // floor
    glBindVertexArray(planeVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, floorTexture);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
//    std::cout << (blinn ? "Blinn-Phong" : "Phong") << std::endl;
}

void BlinnPhongRender::processInput(GLFWwindow *window) {
    CamerRenderA::processInput(window);
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && !blinnKeyPressed)
    {
        blinn = !blinn;
        blinnKeyPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE)
    {
        blinnKeyPressed = false;
    }
}

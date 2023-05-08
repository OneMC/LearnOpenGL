//
//  DepthTest.cpp
//  LearnOpenGL
//
//  Created by 苗超 on 2023/4/17.
//

#include "DepthTest.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

extern void loadTexture(char const * texturePath, unsigned int *texture, int colorType);

void DepthTestRender::setupCube() {
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float cubeVertices[] = {
        // positions         // texture Coords
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
        
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f
    };
    
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);
}

void DepthTestRender::setupPlane() {
    float planeVertices[] = {
        
        // note we set texture Coords higher than 1
        // together with GL_REPEAT as texture wrapping mode
        // this will cause the floor texture to repeat
        
        // positions         // texture Coords
        5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
        
        5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
        5.0f, -0.5f, -5.0f,  2.0f, 2.0f
    };
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);
}

void DepthTestRender::setDepthTest() {
    // 开启深度测试
    glEnable(GL_DEPTH_TEST);
    // 设置深度测试效果
    // 如果该函数不设置默认是：GL_LESS
    // GL_ALWAYS: 全部通过；与关闭深度测试效果相同；glDisable(GL_DEPTH_TEST))
    // GL_NEVER: 全部不通过；
    // GL_LESS  /  GL_GREATER: 小 / 大
    // GL_EQUAL /  GL_NOTEQUAL: 相等 / 不相等
    // GL_LEQUAL/  GL_GEQUAL: 小于等于 / 大于等于
    glDepthFunc(GL_LESS);
    
//    glDepthMask(GL_FALSE); // 只进行深度测试，但不更新深度值
}

void DepthTestRender::loadTex() {
    // load textures
    loadTexture(ResPath("/LearnOpenGL/depth_test/marble.jpg"), &cubeTexture,GL_RGB);
    loadTexture(ResPath("/LearnOpenGL/depth_test/metal.png"), &floorTexture,GL_RGB);
}

void DepthTestRender::setup() {
    setDepthTest();
    setupCube();
    setupPlane();
    loadTex();
    
    _shader = new Shader(ResPath("/LearnOpenGL/depth_test/depth_test_vertex.vs"),
                         ResPath("/LearnOpenGL/depth_test/depth_test_fragment.fs"));
    _shader->use();
    _shader->setInt("texture1", 0);
}

void DepthTestRender::render() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    // GL_DEPTH_BUFFER_BIT 清除上次frame的深度值
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    setupCamera();
    renderCube();
    renderFloor();
}

void DepthTestRender::setupCamera() {
    CamerRenderA::setupCamera();
    glm::mat4 view = viewModel();
    glm::mat4 projection = projectionModel();
    _shader->use();
    _shader->setMat4("view", view);
    _shader->setMat4("projection", projection);
}

void DepthTestRender::renderCube() {
    // cubes
    glBindVertexArray(cubeVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, cubeTexture);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
    _shader->setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
    _shader->setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void DepthTestRender::renderFloor() {
    // floor
    glBindVertexArray(planeVAO);
    glBindTexture(GL_TEXTURE_2D, floorTexture);
    _shader->setMat4("model", glm::mat4(1.0f));
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void DepthTestRender::releaseRes() {
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &planeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &planeVBO);
}

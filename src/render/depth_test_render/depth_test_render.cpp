//
//  depth_test_render.cpp
//  LearnOpenGL
//
//  Created by ONEMC on 2023/7/7.
//

#include "depth_test_render.hpp"
#include "shader_tool.hpp"

void DepthTestRender::setup() {
    VAO = setupCubeVertex();
    planeVAO = setupPlaneVertex();
    cubeTexture = load2DTexture(R::image("marble.jpg"));
    floorTexture = load2DTexture(R::image("metal.png"));
    
    shader = Shader(R::shader("depth_test_render/depth_test_vertex.vsh"),
                    R::shader("depth_test_render/depth_test_fragment.fsh"));
    setupDepthTest();
}

void DepthTestRender::setupDepthTest() {
    // enable depth test
    glEnable(GL_DEPTH_TEST);
    
    // Depth test function
    // default: GL_LESS
    // GL_ALWAYS: The depth test always passes. The same effect as turning off the depth test ->  glDisable(GL_DEPTH_TEST))
    // GL_NEVER: The depth test never passes.
    // GL_LESS  /  GL_GREATER
    // GL_EQUAL /  GL_NOTEQUAL
    // GL_LEQUAL/  GL_GEQUAL
    glDepthFunc(GL_LESS);
    
//    glDepthMask(GL_FALSE); // 只进行深度测试，但不更新深度值
}

void DepthTestRender::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    setupCamera();
    renderCube();
    renderFloor();
}

void DepthTestRender::setupCamera() {
    CameraBaseRender::updateCamera();
    glm::mat4 view = viewModel();
    glm::mat4 projection = projectionModel();
    shader.use();
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
}

void DepthTestRender::renderCube() {
    shader.use();
    shader.setInt("texture1", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, cubeTexture);
    
    glBindVertexArray(VAO);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
    shader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
    shader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void DepthTestRender::renderFloor() {
    glBindVertexArray(planeVAO);
    glBindTexture(GL_TEXTURE_2D, floorTexture);
    shader.setMat4("model", glm::mat4(1.0f));
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

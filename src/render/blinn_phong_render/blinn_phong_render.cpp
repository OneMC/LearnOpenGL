//
//  blinn_phong_render.cpp
//  LearnOpenGL
//
//  Created by MC on 2023/7/13.
//

#include "blinn_phong_render.hpp"
#include "shader_tool.hpp"

extern float planeVertices1[48];

void BlinnPhongRender::setup() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    planeVAO = setupPlaneVertexNomal(planeVertices1, sizeof(float) * 48);
    
    floorTexture = load2DTexture(R::image("wood.png"));
    shader = Shader(R::shader("blinn_phong_render/blinn_phong_vertex.vsh"),
                    R::shader("blinn_phong_render/blinn_phong_fragment.fsh"));
    shader.use();
    shader.setInt("floorTexture", 0);
}

void BlinnPhongRender::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    CameraBaseRender::updateCamera();
    shader.use();
    shader.setMat4("view", viewModel());
    shader.setMat4("projection", projectionModel());
    
    glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
    // set light uniforms
    shader.setVec3("viewPos", camera.Position);
    shader.setVec3("lightPos", lightPos);
    shader.setInt("blinn", blinn);
    
    // floor
    glBindVertexArray(planeVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, floorTexture);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void BlinnPhongRender::process_input(GLFWwindow *window) {
    CameraBaseRender::process_input(window);
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

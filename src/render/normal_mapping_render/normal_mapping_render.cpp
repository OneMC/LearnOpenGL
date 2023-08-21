//
//  normal_mapping_render.cpp
//  LearnOpenGL
//
//  Created by ONEMC on 2023/7/28.
//

#include "normal_mapping_render.hpp"
#include "shader_tool.hpp"

void NormalMappingRender::setup() {
    camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));
    diffuseTexture = load2DTexture(R::image("brickwall.jpg"));
    normalMapTexture = load2DTexture(R::image("brickwall_normal.jpg"));
    shader = Shader(R::shader("normal_mapping_render/normal_mapping_vertex.vsh"),
                    R::shader("normal_mapping_render/normal_mapping_fragment.fsh"));
    // shader configuration
    shader.use();
    shader.setInt("diffuseMap", 0);
    shader.setInt("normalMap", 1);
    glEnable(GL_DEPTH_TEST);
}

void NormalMappingRender::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    updateCamera();
    
    shader.use();
    shader.setMat4("view", viewModel());
    shader.setMat4("projection", projectionModel());
    
    // render normal-mapped quad
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians((float)glfwGetTime() * -10.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0))); // rotate the quad to show normal mapping from multiple directions
    shader.setMat4("model", model);
    shader.setVec3("viewPos", camera.Position);
    shader.setVec3("lightPos", lightPos);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, normalMapTexture);
    renderQuadMapping(quadVAO);
    
    // render light source (simply re-renders a smaller plane at the light's position for debugging/visualization)
    model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.1f));
    shader.setMat4("model", model);
    renderQuadMapping(quadVAO);
}

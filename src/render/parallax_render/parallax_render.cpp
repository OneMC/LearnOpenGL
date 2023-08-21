//
//  parallax_render.cpp
//  LearnOpenGL
//
//  Created by ONEMC on 2023/7/27.
//

#include "parallax_render.hpp"
#include "shader_tool.hpp"

void ParallaxRender::setup() {
    diffuseTexture = load2DTexture(R::image("bricks2.jpg"));
    normalTexture = load2DTexture(R::image("bricks2_normal.jpg"));
    heightTexture = load2DTexture(R::image("bricks2_disp.jpg"));
    
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
    // Post-processing
    const char * parallax_normal = "parallax_render/parallax_mapping_fragment.fsh";
    const char * parallax_steep = "parallax_render/parallax_steep_fragment.fsh";
    const char * parallax_occlusion = "parallax_render/parallax_occlusion_fragment.fsh";
#ifdef __clang__
#pragma clang diagnostic pop
#endif
    
    
    shader = Shader(R::shader("parallax_render/parallax_mapping_vertex.vsh"),
                    R::shader(parallax_occlusion));
    shader.use();
    shader.setInt("diffuseMap", 0);
    shader.setInt("normalMap", 1);
    shader.setInt("depthMap", 2);
    glEnable(GL_DEPTH_TEST);
}


void ParallaxRender::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.use();
    shader.setMat4("projection", projectionModel());
    shader.setMat4("view", viewModel());
    
    // render parallax-mapped quad
    glm::mat4 model = glm::mat4(1.0f);
    // rotate the quad to show parallax mapping from multiple directions
    model = glm::rotate(model, glm::radians((float)glfwGetTime() * -10.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    
    shader.setMat4("model", model);
    shader.setVec3("viewPos", camera.Position);
    shader.setVec3("lightPos", lightPos);
    shader.setFloat("heightScale", heightScale); // adjust with Q and E keys
    std::cout << heightScale << std::endl;
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, normalTexture);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, heightTexture);
    renderQuadMapping(quadVAO);

    // render light source (simply re-renders a smaller plane at the light's position for debugging/visualization)
    model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.1f));
    shader.setMat4("model", model);
    renderQuadMapping(quadVAO);
}

void ParallaxRender::process_input(GLFWwindow *window) {
    CameraBaseRender::process_input(window);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        if (heightScale > 0.0f)
            heightScale -= 0.0005f;
        else
            heightScale = 0.0f;
    } else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        if (heightScale < 1.0f)
            heightScale += 0.0005f;
        else
            heightScale = 1.0f;
    }
}

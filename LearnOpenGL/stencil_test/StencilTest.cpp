//
//  StencilTest.cpp
//  LearnOpenGL
//
//  Created by 苗超 on 2023/4/25.
//

#include "StencilTest.hpp"

void StencilTestRender::setup() {
    setDepthTest();
    setupCube();
    setupPlane();
    loadTex();
    
    // build and compile shaders
    _shader = new Shader(ResPath("/code_resource/stencil_test/stencil_test.vs"),
                         ResPath("/code_resource/stencil_test/stencil_test.fs"));
    _shaderSingleColor = new Shader(ResPath("/code_resource/stencil_test/stencil_test.vs"),
                                    ResPath("/code_resource/stencil_test/stencil_test_single_color.fs"));
    
    // shader configuration
    _shader->use();
    _shader->setInt("texture1", 0);
    
    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
}

void StencilTestRender::render() {
    // render
    // ------
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // don't forget to clear the stencil buffer!
    
    // set uniforms
    setupCamera();
    glm::mat4 view = viewModel();
    glm::mat4 projection = projectionModel();
    
    _shaderSingleColor-> use();
    _shaderSingleColor->setMat4("view", view);
    _shaderSingleColor->setMat4("projection", projection);
    
    _shader->use();
    _shader->setMat4("view", view);
    _shader->setMat4("projection", projection);
    
    // draw floor as normal, but don't write the floor to the stencil buffer, we only care about the containers. We set its mask to 0x00 to not write to the stencil buffer.
    glStencilMask(0x00);
    // floor
    glBindVertexArray(planeVAO);
    glBindTexture(GL_TEXTURE_2D, floorTexture);
    _shader->setMat4("model", glm::mat4(1.0f));
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    
    // 1st. render pass, draw objects as normal, writing to the stencil buffer
    // --------------------------------------------------------------------
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);
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
    
    // 2nd. render pass: now draw slightly scaled versions of the objects, this time disabling stencil writing.
    // Because the stencil buffer is now filled with several 1s. The parts of the buffer that are 1 are not drawn, thus only drawing
    // the objects' size differences, making it look like borders.
    // --------------------------------------------------------------------
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glDisable(GL_DEPTH_TEST);
    _shaderSingleColor->use();
    float scale = 1.1f;
    // cubes
    glBindVertexArray(cubeVAO);
    glBindTexture(GL_TEXTURE_2D, cubeTexture);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
    model = glm::scale(model, glm::vec3(scale, scale, scale));
    _shaderSingleColor->setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(scale, scale, scale));
    _shaderSingleColor->setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    glEnable(GL_DEPTH_TEST);
}

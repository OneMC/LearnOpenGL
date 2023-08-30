//
//  shadow_render.cpp
//  LearnOpenGL
//
//  Created by MC on 2023/7/13.
//

#include "shadow_render.hpp"
#include "shader_tool.hpp"

extern float planeVertices2[48];

#ifdef __APPLE__
static unsigned int Screen_scale = 2; // mac need scale
static unsigned int SHADOW_WIDTH = 1024 * Screen_scale;
static unsigned int SHADOW_HEIGHT = 1024 * Screen_scale;
#else
static unsigned int Screen_scale = 1;
static unsigned int SHADOW_WIDTH = 1024;
static unsigned int SHADOW_HEIGHT = 1024;
#endif

void ShadowRender::setupFBO() {
    // configure depth map FBO
    glGenFramebuffers(1, &depthMapFBO);
    // create depth texture
    glGenTextures(1, &depthMapTexture);
    glBindTexture(GL_TEXTURE_2D, depthMapTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTexture, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowRender::setup() {
    glEnable(GL_DEPTH_TEST);
    shader = Shader(R::shader("shadow_render/shadow_vertex.vsh"),
                    R::shader("shadow_render/shadow_fragment.fsh"));
    createDepthShader = Shader(R::shader("shadow_render/shadow_create_depth_vertex.vsh"),
                               R::shader("shadow_render/shadow_create_depth_fragment.fsh"));
    debugDepthFboShader = Shader(R::shader("shadow_render/shadow_debug_vertex.vsh"),
                                 R::shader("shadow_render/shadow_debug_fragment.fsh"));
    
    shader.use();
    shader.setInt("diffuseTexture", 0); // set image texutre
    shader.setInt("shadowMap", 1); // set depth value
    
    debugDepthFboShader.use();
    debugDepthFboShader.setInt("depthMap", 0);
    
    woodTexture = load2DTexture(R::image("wood.png"));
    
    planeVAO = setupPlaneVertexNomal(planeVertices2, sizeof(float) * 48);
    
    setupFBO();
}

void ShadowRender::render() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // 1. render depth of scene to texture (from light's perspective)
    lightPos = glm::vec3(-2.0f, 4.0f, -1.0f);
    glm::mat4 lightProjection, lightView;
    float near_plane = 1.0f, far_plane = 7.5f;
    lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
    lightSpaceMatrix = lightProjection * lightView;

    renderDepthToTexture();
//    renderDebugDepthMap(near_plane, far_plane);
    renderSceneWithShadow();
}

void ShadowRender::renderDepthToTexture() {
    
    // render scene from light's point of view
    createDepthShader.use();
    createDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
    
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, woodTexture);
    glCullFace(GL_FRONT);
    renderScene(createDepthShader);
    glCheckError();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glCullFace(GL_BACK); // restore culling face
    glCheckError();
}

void ShadowRender::renderSceneWithShadow() {
    // reset viewport
    glViewport(0, 0, SCR_WIDTH * Screen_scale, SCR_HEIGHT * Screen_scale);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    shader.use();
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    
    shader.setMat4("projection", projection); // vs
    shader.setMat4("view", view); //vs
    shader.setMat4("lightSpaceMatrix", lightSpaceMatrix); // vs
    
    // set light uniforms
    shader.setVec3("viewPos", camera.Position); // fs
    shader.setVec3("lightPos", lightPos); // fs
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, woodTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthMapTexture);
    renderScene(shader);
}

void ShadowRender::renderDebugDepthMap(float near_plane, float far_plane) {
    // reset viewport
    glViewport(0, 0, SCR_WIDTH * Screen_scale, SCR_HEIGHT * Screen_scale);
    
    // render Depth map to quad for visual debugging
    debugDepthFboShader.use();
    debugDepthFboShader.setFloat("near_plane", near_plane);
    debugDepthFboShader.setFloat("far_plane", far_plane);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depthMapTexture);
    renderQuad(quadVAO);
}

void ShadowRender::renderScene(Shader& shader) {
    // floor
    glm::mat4 model = glm::mat4(1.0f);
    shader.setMat4("model", model); // vs
    glBindVertexArray(planeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    // cubes
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setMat4("model", model);
    renderCube(cubeVAO);
    
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setMat4("model", model);
    renderCube(cubeVAO);
    
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0));
    model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    model = glm::scale(model, glm::vec3(0.25));
    shader.setMat4("model", model);
    renderCube(cubeVAO);
}

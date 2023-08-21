//
//  hdr_render.cpp
//  LearnOpenGL
//
//  Created by MC on 2023/7/19.
//

#include "hdr_render.hpp"
#include "shader_tool.hpp"

// lighting info
void HDRRender::setupLight() {
    // positions
    lightPositions.push_back(glm::vec3( 0.0f,  0.0f, 49.5f)); // back light
    lightPositions.push_back(glm::vec3(-1.4f, -1.9f, 9.0f));
    lightPositions.push_back(glm::vec3( 0.0f, -1.8f, 4.0f));
    lightPositions.push_back(glm::vec3( 0.8f, -1.7f, 6.0f));
    // colors
    lightColors.push_back(glm::vec3(200.0f, 200.0f, 200.0f));
    lightColors.push_back(glm::vec3(0.1f, 0.0f, 0.0f));
    lightColors.push_back(glm::vec3(0.0f, 0.0f, 0.2f));
    lightColors.push_back(glm::vec3(0.0f, 0.1f, 0.0f));
}

void HDRRender::setupHDRFBO() {
    glGenFramebuffers(1, &hdrFBO);
    
    // create floating point color buffer
    glGenTextures(1, &colorBuffer);
    glBindTexture(GL_TEXTURE_2D, colorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // create depth buffer (render buffer)
    unsigned int depthRbo;
    glGenRenderbuffers(1, &depthRbo);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);
    
    // attach to fbo
    glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRbo);
    
    // check fbo attach result!
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        printf("%s: attach frame buffer error",__func__);
        assert(false);
    }
    
    // unbind framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void HDRRender::setup() {
    camera = Camera(glm::vec3(0.0f, 0.5f, 30.0f));
    // build and compile shaders
    shader = Shader(R::shader("/hdr_render/hdr_lighting_vertex.vsh"),
                    R::shader("/hdr_render/hdr_lighting_fragment.fsh"));
    toneMapShader = Shader(R::shader("/hdr_render/hdr_tone_vertex.vsh"),
                            R::shader("/hdr_render/hdr_tone_fragment.fsh"));;
    
    // shader configuration
    shader.use();
    shader.setInt("diffuseTexture", 0);
    
    toneMapShader.use();
    toneMapShader.setInt("hdrBuffer", 0);
    
    woodTexture = load2DTexture(R::image("wood.png"));
    
    setupLight();
    setupHDRFBO();
    
    glEnable(GL_DEPTH_TEST);
}


void HDRRender::render() {
    // render
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    updateCamera();
    renderHDRLighting();
    renderToneMap();
}

void HDRRender::renderHDRLighting() {
    // 1. render scene into floating point framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glm::mat4 view = viewModel();
    glm::mat4 projection = projectionModel();
    
    shader.use();
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, woodTexture);
    // set lighting uniforms
    for (unsigned int i = 0; i < lightPositions.size(); i++)
    {
        shader.setVec3("lights[" + std::to_string(i) + "].Position", lightPositions[i]);
        shader.setVec3("lights[" + std::to_string(i) + "].Color", lightColors[i]);
    }
    shader.setVec3("viewPos", camera.Position);
    
    // render tunnel
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 25.0));
    model = glm::scale(model, glm::vec3(2.5f, 2.5f, 27.5f));
    shader.setMat4("model", model);
    shader.setInt("inverse_normals", true);
    renderCube(cubeVAO);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void HDRRender::renderToneMap() {
    // 2. now render floating point color buffer to 2D quad and tonemap HDR colors to default framebuffer's (clamped) color range
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    toneMapShader.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, colorBuffer);
    toneMapShader.setInt("hdr", hdrSwitch);
    toneMapShader.setFloat("exposure", exposure);
    renderQuad(quadVAO);
}


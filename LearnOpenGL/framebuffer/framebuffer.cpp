//
//  framebuffer.cpp
//  LearnOpenGL
//
//  Created by 苗超 on 2023/4/28.
//

#include "framebuffer.hpp"
#include <iostream>

extern void loadTexture(char const * texturePath, unsigned int *texture, int colorType);

void FrameBufferRender::setupQuad() {
    // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
    float quadVertices[] = {
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
        1.0f, -1.0f,  1.0f, 0.0f,
        
        -1.0f,  1.0f,  0.0f, 1.0f,
        1.0f, -1.0f,  1.0f, 0.0f,
        1.0f,  1.0f,  1.0f, 1.0f
    };
    
    // screen quad VAO
    unsigned int quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

void FrameBufferRender::setupShader() {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"
    // Post-processing
    const char * normalScreen = "/code_resource/framebuffer/framebuffer_screen.fs";
    const char * inversion = "/code_resource/framebuffer/framebuffer_inversion.fs";
    const char * grayscale = "/code_resource/framebuffer/framebuffer_grayscale.fs";
    const char * kernel = "/code_resource/framebuffer/framebuffer_kernel.fs";
    const char * blur = "/code_resource/framebuffer/framebuffer_blur.fs";
    const char * edgeDetection = "/code_resource/framebuffer/framebuffer_edge_detection.fs";
#pragma clang diagnostic pop
    
    _shader = new Shader(ResPath("/code_resource/framebuffer/framebuffer_vertex.vs"),
                         ResPath("/code_resource/framebuffer/framebuffer_fragment.fs"));
    
    _screenShader = new Shader(ResPath("/code_resource/framebuffer/framebuffer_screen.vs"),
                               ResPath(edgeDetection));
    // _shader->configuration
    // --------------------
    _shader->use();
    _shader->setInt("texture1", 0);
    
    _screenShader->use();
    _screenShader->setInt("screenTexture", 0);
}

void FrameBufferRender::setup() {
    setupCube();
    setupPlane();
    setupQuad();
    loadTex();
    setupShader();
    setupFrameBuffer();
}

void FrameBufferRender::loadTex() {
    loadTexture(ResPath("/LearnOpenGL/texture/container.jpeg"), &cubeTexture,GL_RGB);
    loadTexture(ResPath("/LearnOpenGL/depth_test/metal.png"), &floorTexture,GL_RGB);
}

void FrameBufferRender::setupFrameBuffer() {
    // framebuffer configuration
    // -------------------------
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    
    // create a color attachment texture
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
    
    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT); // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
    
    // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
        assert(false);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBufferRender::render() {
    firstPassRender();
    secondPassRender();
}

void FrameBufferRender::firstPassRender() {
    // bind to framebuffer and draw scene as we normally would to color texture
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)
    
    // render scene
    DepthTestRender::render();
}

void FrameBufferRender::secondPassRender() {
    // now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
    // clear all relevant buffers
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
    glClear(GL_COLOR_BUFFER_BIT);
    
    _screenShader->use();
    glBindVertexArray(quadVAO);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);    // use the color attachment texture as the texture of the quad plane
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

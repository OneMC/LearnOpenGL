//
//  framebuffer_render.cpp
//  LearnOpenGL
//
//  Created by MC on 2023/7/13.
//

#include "framebuffer_render.hpp"
#include "shader_tool.hpp"

void FrameBufferRender::setup() {
    quadVAO = setupQuad();
    VAO = setupCubeVertex();
    planeVAO = setupPlaneVertex();
    
    cubeTexture = load2DTexture(R::image("container.jpg"));
    floorTexture = load2DTexture(R::image("metal.png"));
    setupFrameBuffer();
    setupShader();
}

void FrameBufferRender::setupFrameBuffer() {
    // framebuffer configuration
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

void FrameBufferRender::setupShader() {
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
    // Post-processing
    const char * normalScreen = "framebuffer_render/framebuffer_screen.fsh";
    const char * inversion = "framebuffer_render/framebuffer_inversion.fsh";
    const char * grayscale = "framebuffer_render/framebuffer_grayscale.fsh";
    const char * kernel = "framebuffer_render/framebuffer_kernel.fsh";
    const char * blur = "framebuffer_render/framebuffer_blur.fsh";
    const char * edgeDetection = "framebuffer_render/framebuffer_edge_detection.fsh";
#ifdef __clang__
#pragma clang diagnostic pop
#endif
    
    shader = Shader(R::shader("depth_test_render/depth_test_vertex.vsh"),
                    R::shader("framebuffer_render/framebuffer_fragment.fsh"));
    
    screenShader = Shader(R::shader("framebuffer_render/framebuffer_screen.vsh"),
                          R::shader(edgeDetection));
    
    // _shader->configuration
    shader.use();
    shader.setInt("texture1", 0);
    
    screenShader.use();
    screenShader.setInt("screenTexture", 0);
    glCheckError();
}

void FrameBufferRender::render() {
    firstPassRender();
    secondPassRender();
}

void FrameBufferRender::firstPassRender() {
    // bind to framebuffer and draw scene as we normally would to color texture
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    
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
    
    screenShader.use();
    glBindVertexArray(quadVAO);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);    // use the color attachment texture as the texture of the quad plane
    glDrawArrays(GL_TRIANGLES, 0, 6);
}


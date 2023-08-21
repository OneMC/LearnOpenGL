//
//  deferred_render.cpp
//  LearnOpenGL
//
//  Created by ONEMC on 2023/7/25.
//

#include "deferred_render.hpp"
#include "shader_tool.hpp"

#ifdef __APPLE__
static unsigned int Screen_scale = 2; // mac need scale
static unsigned int _SCREEN_WIDTH = SCR_WIDTH * Screen_scale;
static unsigned int _SCREEN_HEIGHT = SCR_HEIGHT * Screen_scale;
#else
static unsigned int _SCREEN_WIDTH = SCR_WIDTH;
static unsigned int _SCREEN_HEIGHT = SCR_HEIGHT;
#endif

void DeferredRender::setup() {
    setupLightingShader();
    setupGBuffer();
    loadModel();
    setupLight();
    lightBoxShader = Shader(R::shader("deferred_render/deferred_light_box_vertex.vsh"),
                            R::shader("deferred_render/deferred_light_box_fragment.fsh"));
    geometryShader = Shader(R::shader("deferred_render/g_buffer_vertex.vsh"),
                            R::shader("deferred_render/g_buffer_fragment.fsh"));
    glEnable(GL_DEPTH_TEST);
}

void DeferredRender::setupLightingShader() {
    lightingShader = Shader(R::shader("deferred_render/deferred_shading_vertex.vsh"),
                            R::shader("deferred_render/deferred_shading_fragment.fsh"));
    // shader configuration
    lightingShader.use();
    lightingShader.setInt("gPosition", 0);
    lightingShader.setInt("gNormal", 1);
    lightingShader.setInt("gAlbedoSpec", 2);
}

void DeferredVolumeRender::setupLightingShader() {
    lightingShader = Shader(R::shader("deferred_render/deferred_shading_vertex.vsh"),
                            R::shader("deferred_render/deferred_shading_volume_fragment.fsh"));
    // shader configuration
    lightingShader.use();
    lightingShader.setInt("gPosition", 0);
    lightingShader.setInt("gNormal", 1);
    lightingShader.setInt("gAlbedoSpec", 2);
}

void DeferredRender::setupGBuffer() {
    // configure g-buffer framebuffer
    glGenFramebuffers(1, &gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
    
    // position color buffer
    glGenTextures(1, &gPosition);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, _SCREEN_WIDTH, _SCREEN_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
    
    // normal color buffer
    glGenTextures(1, &gNormal);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, _SCREEN_WIDTH, _SCREEN_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
    
    // color + specular color buffer
    glGenTextures(1, &gAlbedoSpec);
    glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _SCREEN_WIDTH, _SCREEN_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);
    // tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
    unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, attachments);
    
    // create and attach depth buffer (renderbuffer)
    unsigned int rboDepth;
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, _SCREEN_WIDTH, _SCREEN_HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
    
    // finally check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DeferredRender::loadModel() {
    // load models
    backpack = Model(R::file("resources/objects/backpack/backpack.obj"));
    objectPositions.push_back(glm::vec3(-3.0,  -0.5, -3.0));
    objectPositions.push_back(glm::vec3( 0.0,  -0.5, -3.0));
    objectPositions.push_back(glm::vec3( 3.0,  -0.5, -3.0));
    objectPositions.push_back(glm::vec3(-3.0,  -0.5,  0.0));
    objectPositions.push_back(glm::vec3( 0.0,  -0.5,  0.0));
    objectPositions.push_back(glm::vec3( 3.0,  -0.5,  0.0));
    objectPositions.push_back(glm::vec3(-3.0,  -0.5,  3.0));
    objectPositions.push_back(glm::vec3( 0.0,  -0.5,  3.0));
    objectPositions.push_back(glm::vec3( 3.0,  -0.5,  3.0));
}

void DeferredRender::setupLight() {
    // lighting info
    const unsigned int NR_LIGHTS = 32;
    srand(13);
    for (unsigned int i = 0; i < NR_LIGHTS; i++)
    {
        // calculate slightly random offsets
        float xPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 3.0);
        float yPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 4.0);
        float zPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 3.0);
        lightPositions.push_back(glm::vec3(xPos, yPos, zPos));
        // also calculate random color
        float rColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.0
        float gColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.0
        float bColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.0
        lightColors.push_back(glm::vec3(rColor, gColor, bColor));
    }
}

void DeferredRender::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    updateCamera();
    renderToGeometryBuffer();
    renderLightWithGBuffer();
    renderLight();
}


void DeferredRender::renderToGeometryBuffer() {
    // 1. geometry pass: render scene's geometry/color data into gbuffer
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glm::mat4 model = glm::mat4(1.0f);
    geometryShader.use();
    geometryShader.setMat4("projection", projectionModel());
    geometryShader.setMat4("view", viewModel());
    for (unsigned int i = 0; i < objectPositions.size(); i++)
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, objectPositions[i]);
        model = glm::scale(model, glm::vec3(0.5f));
        geometryShader.setMat4("model", model);
        backpack.Draw(geometryShader);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void DeferredRender::renderLightWithGBuffer() {
    // 2. lighting pass: calculate lighting by iterating over a screen filled quad pixel-by-pixel using the gbuffer's content.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    lightingShader.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
    
    lightingShader.setVec3("viewPos", camera.Position);
    passLightInfoToShader();
    
    // finally render quad
    renderQuad(quadVAO);

    // 2.5. copy content of geometry's depth buffer to default framebuffer's depth buffer
    glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // write to default framebuffer
    // blit to default framebuffer. Note that this may or may not work as the internal formats of both the FBO and default framebuffer have to match.
    // the internal formats are implementation defined. This works on all of my systems, but if it doesn't on yours you'll likely have to write to the
    // depth buffer in another shader stage (or somehow see to match the default framebuffer's internal format with the FBO's internal format).
    glBlitFramebuffer(0, 0, _SCREEN_WIDTH, _SCREEN_HEIGHT, 0, 0, _SCREEN_WIDTH, _SCREEN_HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void DeferredRender::renderLight() {
    // 3. render lights on top of scene
    // Not for post-processing, just showing the lighting
    lightBoxShader.use();
    lightBoxShader.setMat4("projection", projectionModel());
    lightBoxShader.setMat4("view", viewModel());
    glm::mat4 model = glm::mat4(1.0f);
    for (unsigned int i = 0; i < lightPositions.size(); i++) {
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPositions[i]);
        model = glm::scale(model, glm::vec3(0.125f));
        lightBoxShader.setMat4("model", model);
        lightBoxShader.setVec3("lightColor", lightColors[i]);
        renderCube(cubeVAO);
    }
}

void DeferredRender::passLightInfoToShader() {
    // send light relevant uniforms
    for (unsigned int i = 0; i < lightPositions.size(); i++)  {
        lightingShader.setVec3("lights[" + std::to_string(i) + "].Position", lightPositions[i]);
        lightingShader.setVec3("lights[" + std::to_string(i) + "].Color", lightColors[i]);
        // update attenuation parameters and calculate radius
        const float linear = 0.7f;
        const float quadratic = 1.8f;
        lightingShader.setFloat("lights[" + std::to_string(i) + "].Linear", linear);
        lightingShader.setFloat("lights[" + std::to_string(i) + "].Quadratic", quadratic);
    }
}

void DeferredVolumeRender::passLightInfoToShader() {
    // send light relevant uniforms
    for (unsigned int i = 0; i < lightPositions.size(); i++)
    {
        lightingShader.setVec3("lights[" + std::to_string(i) + "].Position", lightPositions[i]);
        lightingShader.setVec3("lights[" + std::to_string(i) + "].Color", lightColors[i]);
        // update attenuation parameters and calculate radius
        const float constant = 1.0f; // note that we don't send this to the shader, we assume it is always 1.0 (in our case)
        const float linear = 0.7f;
        const float quadratic = 1.8f;
        lightingShader.setFloat("lights[" + std::to_string(i) + "].Linear", linear);
        lightingShader.setFloat("lights[" + std::to_string(i) + "].Quadratic", quadratic);
        // then calculate radius of light volume/sphere
        const float maxBrightness = std::fmaxf(std::fmaxf(lightColors[i].r, lightColors[i].g), lightColors[i].b);
        float radius = (-linear + std::sqrt(linear * linear - 4 * quadratic * (constant - (256.0f / 5.0f) * maxBrightness))) / (2.0f * quadratic);
        lightingShader.setFloat("lights[" + std::to_string(i) + "].Radius", radius);
    }
}

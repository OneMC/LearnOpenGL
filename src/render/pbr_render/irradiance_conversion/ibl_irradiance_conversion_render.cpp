//
//  pbr_irradiance_render.cpp
//  LearnOpenGL
//
//  Created by ONEMC on 2023/8/15.
//

#include "ibl_irradiance_conversion_render.hpp"

void IBLIrradianceConvRender::setup() {
    commonSetup();
    setupStaticShader();
    // configure global opengl state
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL); // set depth function to less than AND equal for skybox depth trick.
}

void IBLIrradianceConvRender::commonSetup() {
    setupCommonShader();
    setupShader();
    setupFrameBuffer();
    hdrTexture = loadHdr(R::file("resources/textures/hdr/newport_loft.hdr"));
    setupEnvCubemap();
    setupCubemap();
}

void IBLIrradianceConvRender::setupCommonShader() {
    equirectangularToCubemapShader = Shader(R::shader("pbr_render/irradiance_conversion/cubemap_vertex.vsh"),
                                            R::shader("pbr_render/irradiance_conversion/equirectangular_to_cubemap_fragment.fsh"));
    backgroundShader = Shader(R::shader("pbr_render/irradiance_conversion/background_vertex.vsh"),
                              R::shader("pbr_render/irradiance_conversion/background_fragment.fsh"));
    
    backgroundShader.use();
    backgroundShader.setInt("environmentMap", 0);
}

void IBLIrradianceConvRender::setupShader() {
    pbrShader = Shader(R::shader("pbr_render/pbr_vertex.vsh"),
                       R::shader("pbr_render/irradiance_conversion/pbr_fragment.fsh"));
    pbrShader.use();
    pbrShader.setVec3("albedo", 0.5f, 0.0f, 0.0f);
    pbrShader.setFloat("ao", 1.0f);
}

void IBLIrradianceConvRender::setupFrameBuffer() {
    // pbr: setup framebuffer
    glGenFramebuffers(1, &captureFBO);
    glGenRenderbuffers(1, &captureRBO);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);
}

void IBLIrradianceConvRender::setupEnvCubemap() {
    // pbr: setup cubemap to render to and attach to framebuffer
    glGenTextures(1, &envCubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
    for (unsigned int i = 0; i < 6; ++i) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void IBLIrradianceConvRender::setupCubemap() {    
    // pbr: convert HDR equirectangular environment map to cubemap equivalent
    equirectangularToCubemapShader.use();
    equirectangularToCubemapShader.setInt("equirectangularMap", 0);
    equirectangularToCubemapShader.setMat4("projection", captureProjection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, hdrTexture);

    glViewport(0, 0, 512, 512); // don't forget to configure the viewport to the capture dimensions.
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    for (unsigned int i = 0; i < 6; ++i) {
        equirectangularToCubemapShader.setMat4("view", captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderCube(cubeVAO);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void IBLIrradianceConvRender::setupStaticShader() {
    // initialize static shader uniforms before rendering
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    pbrShader.use();
    pbrShader.setMat4("projection", projection);
    backgroundShader.use();
    backgroundShader.setMat4("projection", projection);

    // then before rendering, configure the viewport to the original framebuffer's screen dimensions
    int scrWidth, scrHeight;
    glfwGetFramebufferSize(window, &scrWidth, &scrHeight);
    glViewport(0, 0, scrWidth, scrHeight);
}

void IBLIrradianceConvRender::render() {
    updateCamera();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // render scene, supplying the convoluted irradiance map to the final shader.
    // ------------------------------------------------------------------------------------------
    pbrShader.use();
    glm::mat4 view = camera.GetViewMatrix();
    pbrShader.setMat4("view", view);
    pbrShader.setVec3("camPos", camera.Position);
    
    // render rows*column number of spheres with varying metallic/roughness values scaled by rows and columns respectively
    glm::mat4 model = glm::mat4(1.0f);
    for (int row = 0; row < nrRows; ++row) {
        pbrShader.setFloat("metallic", (float)row / (float)nrRows);
        for (int col = 0; col < nrColumns; ++col) {
            // we clamp the roughness to 0.025 - 1.0 as perfectly smooth surfaces (roughness of 0.0) tend to look a bit off
            // on direct lighting.
            pbrShader.setFloat("roughness", glm::clamp((float)col / (float)nrColumns, 0.05f, 1.0f));

            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(
                (float)(col - (nrColumns / 2)) * spacing,
                (float)(row - (nrRows / 2)) * spacing,
                -2.0f
            ));
            pbrShader.setMat4("model", model);
            pbrShader.setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
            renderSphere();
        }
    }


    renderLight();

    // render skybox (render as last to prevent overdraw)
    backgroundShader.use();
    backgroundShader.setMat4("view", view);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
    renderCube(cubeVAO);

    /*    equirectangularToCubemapShader.Use();
    equirectangularToCubemapShader.setMat4("view", view");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, hdrTexture);
    renderCube();*/
}

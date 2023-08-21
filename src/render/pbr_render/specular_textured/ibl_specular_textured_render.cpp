//
//  ibl_specular_textured_render.cpp
//  LearnOpenGL
//
//  Created by ONEMC on 2023/8/18.
//

#include "ibl_specular_textured_render.hpp"

void IBLSpecularTexturedRender::setup() {
    IBLSpecularRender::commonSetup();
    loadTexture();
    setupStaticShader();
    // configure global opengl state
    glEnable(GL_DEPTH_TEST);
    // set depth function to less than AND equal for skybox depth trick.
    glDepthFunc(GL_LEQUAL);
    // enable seamless cubemap sampling for lower mip levels in the pre-filter map.
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    glCheckError();
}

void IBLSpecularTexturedRender::setupShader() {
    pbrShader = Shader(R::shader("pbr_render/pbr_vertex.vsh"),
                       R::shader("pbr_render/specular_textured/pbr_fragment.fsh"));
    pbrShader.use();
    pbrShader.setInt("irradianceMap", 0);
    pbrShader.setInt("prefilterMap", 1);
    pbrShader.setInt("brdfLUT", 2);
    pbrShader.setInt("albedoMap", 3);
    pbrShader.setInt("normalMap", 4);
    pbrShader.setInt("metallicMap", 5);
    pbrShader.setInt("roughnessMap", 6);
    pbrShader.setInt("aoMap", 7);
}

void IBLSpecularTexturedRender::loadTexture() {
    // load PBR material textures
    // rusted iron
    ironAlbedoMap = load2DTexture(R::file("resources/textures/pbr/rusted_iron/albedo.png"));
    ironNormalMap = load2DTexture(R::file("resources/textures/pbr/rusted_iron/normal.png"));
    ironMetallicMap = load2DTexture(R::file("resources/textures/pbr/rusted_iron/metallic.png"));
    ironRoughnessMap = load2DTexture(R::file("resources/textures/pbr/rusted_iron/roughness.png"));
    ironAOMap = load2DTexture(R::file("resources/textures/pbr/rusted_iron/ao.png"));

    // gold
    goldAlbedoMap = load2DTexture(R::file("resources/textures/pbr/gold/albedo.png"));
    goldNormalMap = load2DTexture(R::file("resources/textures/pbr/gold/normal.png"));
    goldMetallicMap = load2DTexture(R::file("resources/textures/pbr/gold/metallic.png"));
    goldRoughnessMap = load2DTexture(R::file("resources/textures/pbr/gold/roughness.png"));
    goldAOMap = load2DTexture(R::file("resources/textures/pbr/gold/ao.png"));

    // grass
    grassAlbedoMap = load2DTexture(R::file("resources/textures/pbr/grass/albedo.png"));
    grassNormalMap = load2DTexture(R::file("resources/textures/pbr/grass/normal.png"));
    grassMetallicMap = load2DTexture(R::file("resources/textures/pbr/grass/metallic.png"));
    grassRoughnessMap = load2DTexture(R::file("resources/textures/pbr/grass/roughness.png"));
    grassAOMap = load2DTexture(R::file("resources/textures/pbr/grass/ao.png"));

    // plastic
    plasticAlbedoMap = load2DTexture(R::file("resources/textures/pbr/plastic/albedo.png"));
    plasticNormalMap = load2DTexture(R::file("resources/textures/pbr/plastic/normal.png"));
    plasticMetallicMap = load2DTexture(R::file("resources/textures/pbr/plastic/metallic.png"));
    plasticRoughnessMap = load2DTexture(R::file("resources/textures/pbr/plastic/roughness.png"));
    plasticAOMap = load2DTexture(R::file("resources/textures/pbr/plastic/ao.png"));

    // wall
    wallAlbedoMap = load2DTexture(R::file("resources/textures/pbr/wall/albedo.png"));
    wallNormalMap = load2DTexture(R::file("resources/textures/pbr/wall/normal.png"));
    wallMetallicMap = load2DTexture(R::file("resources/textures/pbr/wall/metallic.png"));
    wallRoughnessMap = load2DTexture(R::file("resources/textures/pbr/wall/roughness.png"));
    wallAOMap = load2DTexture(R::file("resources/textures/pbr/wall/ao.png"));
}

void IBLSpecularTexturedRender::render() {
    updateCamera();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // render scene, supplying the convoluted irradiance map to the final shader.
    // ------------------------------------------------------------------------------------------
    pbrShader.use();
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = camera.GetViewMatrix();
    pbrShader.setMat4("view", view);
    pbrShader.setVec3("camPos", camera.Position);

    // bind pre-computed IBL data
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);

    // rusted iron
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, ironAlbedoMap);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, ironNormalMap);
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, ironMetallicMap);
    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_2D, ironRoughnessMap);
    glActiveTexture(GL_TEXTURE7);
    glBindTexture(GL_TEXTURE_2D, ironAOMap);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-5.0, 0.0, 2.0));
    pbrShader.setMat4("model", model);
    pbrShader.setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
    renderSphere();

    // gold
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, goldAlbedoMap);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, goldNormalMap);
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, goldMetallicMap);
    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_2D, goldRoughnessMap);
    glActiveTexture(GL_TEXTURE7);
    glBindTexture(GL_TEXTURE_2D, goldAOMap);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-3.0, 0.0, 2.0));
    pbrShader.setMat4("model", model);
    pbrShader.setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
    renderSphere();

    // grass
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, grassAlbedoMap);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, grassNormalMap);
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, grassMetallicMap);
    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_2D, grassRoughnessMap);
    glActiveTexture(GL_TEXTURE7);
    glBindTexture(GL_TEXTURE_2D, grassAOMap);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.0, 0.0, 2.0));
    pbrShader.setMat4("model", model);
    pbrShader.setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
    renderSphere();

    // plastic
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, plasticAlbedoMap);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, plasticNormalMap);
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, plasticMetallicMap);
    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_2D, plasticRoughnessMap);
    glActiveTexture(GL_TEXTURE7);
    glBindTexture(GL_TEXTURE_2D, plasticAOMap);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(1.0, 0.0, 2.0));
    pbrShader.setMat4("model", model);
    pbrShader.setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
    renderSphere();

    // wall
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, wallAlbedoMap);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, wallNormalMap);
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, wallMetallicMap);
    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_2D, wallRoughnessMap);
    glActiveTexture(GL_TEXTURE7);
    glBindTexture(GL_TEXTURE_2D, wallAOMap);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(3.0, 0.0, 2.0));
    pbrShader.setMat4("model", model);
    pbrShader.setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
    renderSphere();

    renderLight();

    // render skybox (render as last to prevent overdraw)
    backgroundShader.use();

    backgroundShader.setMat4("view", view);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
    //glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap); // display irradiance map
    //glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap); // display prefilter map
    renderCube(cubeVAO);

    // render BRDF map to screen
    //brdfShader.Use();
    //renderQuad();
}

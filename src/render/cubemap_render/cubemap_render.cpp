//
//  cubemap_render.cpp
//  LearnOpenGL
//
//  Created by ONEMC on 2023/7/24.
//

#include "cubemap_render.hpp"
#include "shader_tool.hpp"
using namespace std;

void CubeMapRender::setup() {
    VAO = setupCubeVertex();
    skyboxVAO = setupSkyBox();
    
    cubeTexture = load2DTexture(R::image("container.jpg"));
    vector<string> faces {
        R::image("skybox/right.jpg"),
        R::image("skybox/left.jpg"),
        R::image("skybox/top.jpg"),
        R::image("skybox/bottom.jpg"),
        R::image("skybox/front.jpg"),
        R::image("skybox/back.jpg")
    };
    cubemapTexture = loadCubemap(faces);
    
    setupSceneShader();
    
    skyboxShader = Shader(R::shader("cubemap_render/skybox_vertex.vsh"),
                          R::shader("cubemap_render/skybox_fragment.fsh"));
    
    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);
    glEnable(GL_DEPTH_TEST);
}

void CubeMapRender::setupSceneShader() {
    shader = Shader(R::shader("cubemap_render/cubemap_vertex.vsh"),
                    R::shader("cubemap_render/cubemap_fragment.fsh"));
    shader.use();
    shader.setInt("texture1", 0);
}

void CubeMapRender::render() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    updateCamera();
    renderScene();
    renderSkybox();
}


// draw scene as normal
void CubeMapRender::renderScene() {
    shader.use();
    
    shader.setMat4("model", glm::mat4(1.0f));
    shader.setMat4("view", viewModel());
    shader.setMat4("projection", projectionModel());
    // cubes
    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, cubeTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}


void CubeMapRender::renderSkybox() {
    // draw skybox as last
    glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
    skyboxShader.use();
    glm::mat4 view = glm::mat4(glm::mat3(viewModel())); // remove translation from the view matrix
    skyboxShader.setMat4("view", view);
    skyboxShader.setMat4("projection", projectionModel());
    // skybox cube
    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS); // set depth function back to default
}

void EnvrMapRender::setupSceneShader() {
    VAO = setupCubeVertexNormal(); // texture coord -> normal value
    shader = Shader(R::shader("cubemap_render/environment_map_vertex.vsh"),
                    R::shader("cubemap_render/environment_map_fragment.fsh"));
    shader.use();
    shader.setInt("skybox", 0);
}

void EnvrMapRender::renderScene() {
    shader.use();
    
    shader.setMat4("model", glm::mat4(1.0f));
    shader.setMat4("view", viewModel());
    shader.setMat4("projection", projectionModel());
    // cubes
    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    // [Differences]: use cubemap texture, not cube texture
    glBindTexture(GL_TEXTURE_2D, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}


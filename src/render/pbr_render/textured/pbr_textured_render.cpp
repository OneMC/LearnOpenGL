//
//  pbr_textured.cpp
//  LearnOpenGL
//
//  Created by ONEMC on 2023/8/15.
//

#include "pbr_textured_render.hpp"

void PBRTextureRender::setup() {
    pbrShader = Shader(R::shader("pbr_render/pbr_vertex.vsh"),
                    R::shader("pbr_render/textured/pbr_texture_fragment.fsh"));
    pbrShader.use();
    pbrShader.setInt("albedoMap", 0);
    pbrShader.setInt("normalMap", 1);
    pbrShader.setInt("metallicMap", 2);
    pbrShader.setInt("roughnessMap", 3);
    pbrShader.setInt("aoMap", 4);
    pbrShader.setMat4("projection", projectionModel());
    
    // load PBR material textures
    albedo = load2DTexture(R::image("pbr/rusted_iron/albedo.png"));
    normal = load2DTexture(R::image("pbr/rusted_iron/normal.png"));
    metallic = load2DTexture(R::image("pbr/rusted_iron/metallic.png"));
    roughness = load2DTexture(R::image("pbr/rusted_iron/roughness.png"));
    ao = load2DTexture(R::image("pbr/rusted_iron/ao.png"));
    
    // configure global opengl state
    glEnable(GL_DEPTH_TEST);
}

void PBRTextureRender::render() {
    updateCamera();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    pbrShader.use();
    glm::mat4 view = camera.GetViewMatrix();
    pbrShader.setMat4("view", view);
    pbrShader.setVec3("camPos", camera.Position);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, albedo);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, normal);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, metallic);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, roughness);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, ao);

    // render rows*column number of spheres with material properties defined by textures (they all have the same material properties)
    glm::mat4 model = glm::mat4(1.0f);
    for (int row = 0; row < nrRows; ++row) {
        for (int col = 0; col < nrColumns; ++col)  {
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(
                (float)(col - (nrColumns / 2)) * spacing,
                (float)(row - (nrRows / 2)) * spacing,
                0.0f
            ));
            pbrShader.setMat4("model", model);
            pbrShader.setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
            renderSphere();
        }
    }
    
    renderLight();
}

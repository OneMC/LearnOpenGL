//
//  model_render.cpp
//  LearnOpenGL
//
//  Created by MC on 2023/7/13.
//

#include "model_render.hpp"


void ModelRender::setup() {
    glEnable(GL_DEPTH_TEST);
    shader = Shader(R::shader("depth_test_render/depth_test_vertex.vsh"),
                    R::shader("model_render/model_fragment.fsh"));
    model = Model(R::file("resources/objects/backpack/backpack.obj"));
}

void ModelRender::render() {
    // render
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    shader.use();
    
    // view/projection transformations
    shader.setMat4("projection", projectionModel());
    shader.setMat4("view", viewModel());
    
    // render the loaded model
    shader.setMat4("model", glm::mat4(1.0f));
    model.Draw(shader);
    glCheckError();
}


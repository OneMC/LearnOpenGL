//
//  geometry_render.cpp
//  LearnOpenGL
//
//  Created by ONEMC on 2023/7/28.
//

#include "geometry_render.hpp"
#include "shader_tool.hpp"

void GeometryRender::setup() {
    VAO = setupFourVertex();
    shader = Shader(R::shader("geometry_render/house_shader/geometry_house_vertex.vsh"),
                    R::shader("geometry_render/house_shader/geometry_house_fragment.fsh"),
                    R::shader("geometry_render/house_shader/geometry_house_geometry.gsh"));
    glEnable(GL_DEPTH_TEST);
}

void GeometryRender::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader.use();
    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, 4);
}

void GeometryExplodingRender::setup() {
    shader = Shader(R::shader("geometry_render/exploding_shader/geometry_exploding_vertex.vsh"),
                    R::shader("geometry_render/exploding_shader/geometry_exploding_fragment.fsh"),
                    R::shader("geometry_render/exploding_shader/geometry_exploding_geometry.gsh"));
    
    // use 'ModelRender' show the 'normal' man
    nanosuit = Model(R::file("resources/objects/nanosuit/nanosuit.obj"));
    glEnable(GL_DEPTH_TEST);
}

void GeometryExplodingRender::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader.use();
    shader.setMat4("projection", projectionModel());
    shader.setMat4("view", viewModel());
    shader.setMat4("model", glm::mat4(1.0f));
    
    // add time component to geometry shader in the form of a uniform
    shader.setFloat("time", static_cast<float>(glfwGetTime()));
    
    // draw model
    nanosuit.Draw(shader);
}

void GeometryNormalVizRender::setup() {
    shader = Shader(R::shader("geometry_render/normals_viz_shader/default_vertex.vsh"),
                    R::shader("geometry_render/normals_viz_shader/default_fragment.fsh"));
    normalShader =
    Shader(R::shader("geometry_render/normals_viz_shader/normal_visualization_vertex.vsh"),
           R::shader("geometry_render/normals_viz_shader/normal_visualization_fragment.fsh"),
           R::shader("geometry_render/normals_viz_shader/normal_visualization_geometry.gsh"));
    backpack = Model(R::file("resources/objects/backpack/backpack.obj"));
}

void GeometryNormalVizRender::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.use();
    shader.setMat4("projection", projectionModel());
    shader.setMat4("view", viewModel());
    shader.setMat4("model", glm::mat4(1.0f));

    // draw model as usual
    backpack.Draw(shader);

    // then draw model with normal visualizing geometry shader
    normalShader.use();
    normalShader.setMat4("projection", projectionModel());
    normalShader.setMat4("view", viewModel());
    normalShader.setMat4("model", glm::mat4(1.0f));

    backpack.Draw(normalShader);
}

//
//  instance_asteroid_render.cpp
//  LearnOpenGL
//
//  Created by ONEMC on 2023/7/28.
//

#include "instance_asteroid_render.hpp"

void AsteroidRender::setup() {
    camera = Camera(glm::vec3(0.0f, 0.0f, 55.0f));
    camera.MovementSpeed *= 50;
    shader = Shader(R::shader("instance_render/asteroid/simple_vertex.vsh"),
                    R::shader("instance_render/asteroid/simple_fragment.fsh"));
    rock = Model(R::file("resources/objects/rock/rock.obj"));
    planet = Model(R::file("resources/objects/planet/planet.obj"));
    setupModelMatrix();
    glEnable(GL_DEPTH_TEST);
}

void AsteroidRender::setupModelMatrix() {
    modelMatrices = new glm::mat4[amount];
    srand(static_cast<unsigned int>(glfwGetTime())); // initialize random seed
    float radius = 150.0f;
    float offset = 25.0f;
    for (unsigned int i = 0; i < amount; i++) {
        glm::mat4 model = glm::mat4(1.0f);
        // 1. translation: displace along circle with 'radius' in range [-offset, offset]
        float angle = (float)i / (float)amount * 360.0f;
        float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float x = sin(angle) * radius + displacement;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float y = displacement * 0.4f; // keep height of asteroid field smaller compared to width of x and z
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float z = cos(angle) * radius + displacement;
        model = glm::translate(model, glm::vec3(x, y, z));

        // 2. scale: Scale between 0.05 and 0.25f
        float scale = static_cast<float>((rand() % 20) / 100.0 + 0.05);
        model = glm::scale(model, glm::vec3(scale));

        // 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
        float rotAngle = static_cast<float>((rand() % 360));
        model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

        // 4. now add to list of matrices
        modelMatrices[i] = model;
    }
}

void AsteroidRender::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    updateCamera();
    
    // configure transformation matrices
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
    shader.use();
    shader.setMat4("projection", projection);
    shader.setMat4("view", viewModel());

    // draw planet
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
    model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
    shader.setMat4("model", model);
    planet.Draw(shader);

    // draw meteorites
    for (unsigned int i = 0; i < amount; i++) {
        shader.setMat4("model", modelMatrices[i]);
        rock.Draw(shader);
    }
}

void InstanceAsteroidRender::setup() {
    camera = Camera(glm::vec3(0.0f, 0.0f, 55.0f));
    camera.MovementSpeed *= 50;
    
    rockShader = Shader(R::shader("instance_render/asteroid/asteroids_vertex.vsh"),
                        R::shader("instance_render/asteroid/simple_fragment.fsh"));
    planetShader = Shader(R::shader("instance_render/asteroid/simple_vertex.vsh"),
                          R::shader("instance_render/asteroid/simple_fragment.fsh"));
    rock = Model(R::file("resources/objects/rock/rock.obj"));
    planet = Model(R::file("resources/objects/planet/planet.obj"));
    setupModelMatrix();
    setupInstancedArray();
    glEnable(GL_DEPTH_TEST);
}

void InstanceAsteroidRender::setupInstancedArray() {

    // configure instanced array
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

    // set transformation matrices as an instance vertex attribute (with divisor 1)
    // note: we're cheating a little by taking the, now publicly declared, VAO of the model's mesh(es) and adding new vertexAttribPointers
    // normally you'd want to do this in a more organized fashion, but for learning purposes this will do.
    for (unsigned int i = 0; i < rock.meshes.size(); i++)
    {
        unsigned int VAO = rock.meshes[i].VAO;
        glBindVertexArray(VAO);
        // set attribute pointers for matrix (4 times vec4)
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glBindVertexArray(0);
    }
}

void InstanceAsteroidRender::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    updateCamera();

    
    // configure transformation matrices
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
    
    // draw planet
    planetShader.use();
    planetShader.setMat4("projection", projection);
    planetShader.setMat4("view", viewModel());
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
    model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
    planetShader.setMat4("model", model);
    planet.Draw(planetShader);

    // draw meteorites
    rockShader.use();
    rockShader.setMat4("projection", projection);
    rockShader.setMat4("view", viewModel());
    rockShader.setInt("texture_diffuse1", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, rock.textures_loaded[0].id); // note: we also made the textures_loaded vector public (instead of private) from the model class.
    for (unsigned int i = 0; i < rock.meshes.size(); i++)
    {
        glBindVertexArray(rock.meshes[i].VAO);
        glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(rock.meshes[i].indices.size()), GL_UNSIGNED_INT, 0, amount);
        glBindVertexArray(0);
    }
    glCheckError();
}

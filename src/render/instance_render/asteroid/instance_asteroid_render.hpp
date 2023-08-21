//
//  instance_asteroid_render.hpp
//  LearnOpenGL
//
//  Created by ONEMC on 2023/7/28.
//

#ifndef instance_asteroid_render_hpp
#define instance_asteroid_render_hpp

#include "camera_base_render.hpp"
#include "model.h"

class AsteroidRender: public CameraBaseRender {
public:
    void setup() override;
    void render() override;
    ~AsteroidRender(){}

protected:
    unsigned int amount = 1000;
    glm::mat4* modelMatrices;
    Model rock, planet;
    void setupModelMatrix();
};

class InstanceAsteroidRender: public AsteroidRender {
public:
    void setup() override;
    void render() override;
    ~InstanceAsteroidRender(){}

private:
    Shader rockShader, planetShader;
    void setupInstancedArray();
};

#endif /* instance_asteroid_render_hpp */

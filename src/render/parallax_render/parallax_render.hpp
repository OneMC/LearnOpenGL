//
//  parallax_render.hpp
//  LearnOpenGL
//
//  Created by ONEMC on 2023/7/27.
//

#ifndef parallax_render_hpp
#define parallax_render_hpp

#include "camera_base_render.hpp"

class ParallaxRender: public CameraBaseRender {
public:
    void setup() override;
    void render() override;
    virtual void process_input(GLFWwindow *window) override;
    ~ParallaxRender(){}

private:
    float heightScale = 0.1f;
    unsigned int quadVAO, cubeVAO;
    unsigned int diffuseTexture=0, normalTexture=0, heightTexture=0;
    glm::vec3 lightPos = glm::vec3(0.5f, 1.0f, 0.3f);
};

#endif /* parallax_render_hpp */

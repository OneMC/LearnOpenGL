//
//  pbr_lighting.hpp
//  LearnOpenGL
//
//  Created by ONEMC on 2023/8/14.
//

#ifndef pbr_lighting_hpp
#define pbr_lighting_hpp

#include "camera_base_render.hpp"

class PBRLightRender: public CameraBaseRender {
public:
    void setup() override;
    void render() override;
    ~PBRLightRender(){}

protected:
    Shader pbrShader;
    // lights
    glm::vec3 lightPositions[4] = {
        glm::vec3(-10.0f,  10.0f, 10.0f),
        glm::vec3( 10.0f,  10.0f, 10.0f),
        glm::vec3(-10.0f, -10.0f, 10.0f),
        glm::vec3( 10.0f, -10.0f, 10.0f),
    };
    glm::vec3 lightColors[4] = {
        glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f)
    };
    
    int nrRows = 7,nrColumns = 7;
    float spacing = 2.5;
    
    unsigned int sphereVAO=0, indexCount=0;
    void renderSphere();
    void renderLight();
};

#endif /* pbr_lighting_hpp */

//
//  pbr_textured.hpp
//  LearnOpenGL
//
//  Created by ONEMC on 2023/8/15.
//

#ifndef pbr_textured_hpp
#define pbr_textured_hpp

#include "pbr_lighting_render.hpp"

class PBRTextureRender: public PBRLightRender {
public:
    void setup() override;
    void render() override;
    ~PBRTextureRender(){}

private:
    unsigned int albedo=0, normal=0, metallic=0, roughness=0, ao=0;
    
    // lights
    glm::vec3 lightPositions[1] = {
        glm::vec3(0.0f, 0.0f, 10.0f),
    };
    glm::vec3 lightColors[1] = {
        glm::vec3(150.0f, 150.0f, 150.0f),
    };
};


#endif /* pbr_textured_hpp */

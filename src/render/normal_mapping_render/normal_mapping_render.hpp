//
//  normal_render.hpp
//  LearnOpenGL
//
//  Created by ONEMC on 2023/7/27.
//

#ifndef normal_render_hpp
#define normal_render_hpp

#include "camera_base_render.hpp"

class NormalMappingRender: public CameraBaseRender {
public:
    void setup() override;
    void render() override;
    ~NormalMappingRender(){}
    
private:
    glm::vec3 lightPos = glm::vec3(0.5f, 1.0f, 0.3f);
    unsigned int quadVAO=0;
    unsigned int diffuseTexture=0, normalMapTexture=0;
};

#endif /* normal_render_hpp */

//
//  shadow.hpp
//  LearnOpenGL
//
//  Created by 苗超 on 2023/5/2.
//

#ifndef shadow_hpp
#define shadow_hpp

#include "blinnePhone.hpp"

class ShadowRender: public CamerRenderA {

public:
    virtual void setup() override;
    virtual void render() override;
    ~ShadowRender(){}
    
private:
    unsigned int woodTexture;
    
    unsigned int depthMapFBO;
    unsigned int depthMap;
    void setupFbo();
    
    Shader *_shader = nullptr;
    Shader *_debugDepthQuad = nullptr;
    Shader *_simpleDepthShader = nullptr;
    
    void renderSceneWithShadow(glm::vec3 lightPos, glm::mat4 lightSpaceMatrix);
    void renderDebugDepthMap(float near_plane, float far_plane);
};


#endif /* shadow_hpp */

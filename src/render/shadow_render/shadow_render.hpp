//
//  shadow_render.hpp
//  LearnOpenGL
//
//  Created by MC on 2023/7/13.
//

#ifndef shadow_render_hpp
#define shadow_render_hpp

#include "camera_base_render.hpp"

class ShadowRender: public CameraBaseRender {
public:
    void setup() override;
    void render() override;
    ~ShadowRender(){}
    
private:
    unsigned int depthMapTexture = 0, depthMapFBO = 0;
    void setupFBO();
    
    unsigned int woodTexture = 0;
    unsigned int cubeVAO = 0, planeVAO = 0, quadVAO = 0;
    
    // lighting info
    glm::vec3 lightPos;
    glm::mat4 lightSpaceMatrix;
    
    Shader debugDepthFboShader;
    Shader createDepthShader;
    
    void renderDepthToTexture();
    void renderSceneWithShadow();
    void renderDebugDepthMap(float near_plane, float far_plane);
    void renderScene(Shader& shader);
};

#endif /* shadow_render_hpp */

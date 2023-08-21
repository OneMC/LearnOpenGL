//
//  bloom_render.hpp
//  LearnOpenGL
//
//  Created by MC on 2023/7/19.
//

#ifndef bloom_render_hpp
#define bloom_render_hpp

#include "camera_base_render.hpp"

class BloomRender: public CameraBaseRender {
public:
    void setup() override;
    void render() override;
    ~BloomRender(){}

private:
    Shader lightShader;
    Shader blurShader;
    Shader finalShader;
    
    std::vector<glm::vec3> lightPositions;
    std::vector<glm::vec3> lightColors;
  
    unsigned int cubeVAO=0, quadVAO=0;
    unsigned int hdrFBO=0;
    unsigned int woodTexture=0, containerTexture=0;
    
    unsigned int colorBuffers[2];
    unsigned int pingpongFBO[2];
    unsigned int pingpongColorbuffers[2];
    
    bool bloom = true;
    float exposure = 1.0f;
    
    void setupShader();
    void setupBloomFbo();
    void setupPingPongFbo();
    void setupLight();
    
    void renderMRTForBloom();
    bool renderForGaussianBlur();
    void renderComposeTwoFbo(bool horizontal);
};

#endif /* bloom_render_hpp */

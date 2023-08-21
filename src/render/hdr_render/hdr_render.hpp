//
//  hdr_render.hpp
//  LearnOpenGL
//
//  Created by MC on 2023/7/19.
//

#ifndef hdr_render_hpp
#define hdr_render_hpp

#include "camera_base_render.hpp"

class HDRRender: public CameraBaseRender {
public:
    void setup() override;
    void render() override;
    ~HDRRender(){}

private:
    
    bool hdrSwitch = true;
    float exposure = 0.1;
    
    unsigned int cubeVAO=0, quadVAO=0;
    
    unsigned int hdrFBO=0, colorBuffer=0;
    unsigned int woodTexture=0;
    Shader toneMapShader;
    
    std::vector<glm::vec3> lightPositions;
    std::vector<glm::vec3> lightColors;
    void setupHDRFBO();
    void setupLight();
    
    void renderHDRLighting();
    void renderToneMap();
};

#endif /* hdr_render_hpp */

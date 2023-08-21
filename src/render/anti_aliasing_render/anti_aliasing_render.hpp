//
//  anti_aliasing_render.hpp
//  LearnOpenGL
//
//  Created by ONEMC on 2023/8/2.
//

#ifndef anti_aliasing_render_hpp
#define anti_aliasing_render_hpp

#include "camera_base_render.hpp"

class AntiAliasingMSAARender: public CameraBaseRender {
public:
    void setup() override;
    void render() override;
    ~AntiAliasingMSAARender(){}
};

class AntiAliasingOffScreenRender: public CameraBaseRender {
public:
    void setup() override;
    void render() override;
    ~AntiAliasingOffScreenRender(){}

private:
    unsigned int cubeVAO=0, quadVAO=0;
    
    unsigned int framebuffer=0;
    unsigned int intermediateFBO=0;
    unsigned int screenTexture=0;
    
    Shader screenShader;
    void setupFrameBuffer();
};

#endif /* anti_aliasing_render_hpp */

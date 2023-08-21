//
//  framebuffer_render.hpp
//  LearnOpenGL
//
//  Created by MC on 2023/7/13.
//

#ifndef framebuffer_render_hpp
#define framebuffer_render_hpp

#include "depth_test_render.hpp"

class FrameBufferRender: public DepthTestRender {
public:
    void setup() override;
    void render() override;
    ~FrameBufferRender(){}
    
private:
    unsigned int framebuffer = 0;
    unsigned int textureColorbuffer = 0;
    unsigned int rbo = 0;
    
    unsigned int quadVAO = 0;
    
    Shader screenShader;
    
    void setupFrameBuffer();
    void firstPassRender();
    void secondPassRender();
    void setupShader();
};

#endif /* framebuffer_render_hpp */

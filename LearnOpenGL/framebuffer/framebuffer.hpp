//
//  framebuffer.hpp
//  LearnOpenGL
//
//  Created by 苗超 on 2023/4/28.
//

#ifndef framebuffer_hpp
#define framebuffer_hpp

#include "DepthTest.hpp"
class FrameBufferRender : public DepthTestRender {
public:
    virtual void setup() override;
    virtual void render() override;

private:
    virtual void loadTex() override;
    
    Shader *_screenShader;
    unsigned int quadVAO;
    void setupQuad();
    
    void setupShader();
    
    unsigned int framebuffer;
    unsigned int textureColorbuffer;
    unsigned int rbo;
    void setupFrameBuffer();
    void firstPassRender();
    void secondPassRender();
};

#endif /* framebuffer_hpp */

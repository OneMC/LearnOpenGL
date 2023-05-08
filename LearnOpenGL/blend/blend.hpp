//
//  blend.hpp
//  LearnOpenGL
//
//  Created by 苗超 on 2023/4/27.
//

#ifndef blend_hpp
#define blend_hpp

#include <stdio.h>
#include "DepthTest.hpp"

class BlendRender : public DepthTestRender {
public:
    virtual void setup() override;
    virtual void render() override;
    ~BlendRender(){}
private:
    unsigned int transparentVBO;
    unsigned int transparentVAO;
    std::vector<glm::vec3> transparentPose;
    void setupTransparentVAO();
    
    unsigned int vegetationTexture;
    void renderVegetation();
    
    unsigned int windowTexture;
    void renderWindow();
    void renderWindowsDepthError();
    
    void setupBlend();
    virtual void loadTex() override;
};


#endif /* blend_hpp */

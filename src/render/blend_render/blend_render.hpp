//
//  blend_render.hpp
//  LearnOpenGL
//
//  Created by MC on 2023/7/10.
//

#ifndef blend_render_hpp
#define blend_render_hpp

#include "depth_test_render.hpp"

class BlendRender: public DepthTestRender {
public:
    void setup() override;
    void render() override;
    ~BlendRender(){}
    
private:
    std::vector<glm::vec3> objectsLocation;
    unsigned int transparentVBO = 0, transparentVAO = 0;
    std::vector<glm::vec3> transparentPose;
    void setupTransparentVertex();
    
    unsigned int vegetationTexture = 0;
    void renderVegetation();
    
    unsigned int windowTexture = 0;
    void renderWindow();
    void renderWindowsDepthError();
};

#endif /* blend_render_hpp */

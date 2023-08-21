//
//  stencil_test_render.hpp
//  LearnOpenGL
//
//  Created by MC on 2023/7/10.
//

#ifndef stencil_test_render_hpp
#define stencil_test_render_hpp

#include "depth_test_render.hpp"

class StencilTestRender: public DepthTestRender {
public:
    void setup() override;
    void render() override;
    
private:
    void setupStencilTest();
    Shader singleColorShader;
    
    void renderObjectFirst();
    void renderObjectSecond();
    void renderFloor();
};

#endif /* stencil_test_render_hpp */

//
//  texturerender.hpp
//  LearnOpenGL
//
//  Created by ONEMC on 2023/7/6.
//

#ifndef texturerender_hpp
#define texturerender_hpp

#include "render.hpp"

class TextureRender : public Render {
public:
    void setup() override;
    void render() override;

private:
    void setupVertex();
    unsigned int containerT=0, awesomeT=0;
};

#endif /* texturerender_hpp */

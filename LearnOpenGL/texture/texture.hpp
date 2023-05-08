//
//  texture.hpp
//  TestOpengl
//
//  Created by 苗超 on 2023/3/7.
//

#ifndef texture_hpp
#define texture_hpp

#include <stdio.h>
#include "Render.hpp"
#include "Shader.h"

class Texture : public Render {
public:
    void setup();
    void render();
    ~Texture();
private:
    Shader *_shader = nullptr;
};

#endif /* texture_hpp */

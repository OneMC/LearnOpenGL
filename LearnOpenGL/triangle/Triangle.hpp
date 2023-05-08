//
//  Triangle.hpp
//  LearnOpenGL
//
//  Created by 苗超 on 2023/3/23.
//

#ifndef Triangle_hpp
#define Triangle_hpp

#include <stdio.h>
#include "Render.hpp"
class Triangle : public Render {
public:
    void setup();
    void render();
    ~Triangle(){};
};

#endif /* Triangle_hpp */

//
//  StencilTest.hpp
//  LearnOpenGL
//
//  Created by 苗超 on 2023/4/25.
//

#ifndef StencilTest_hpp
#define StencilTest_hpp

#include <stdio.h>
#include "DepthTest.hpp"

class StencilTestRender : public DepthTestRender {
public:
    virtual void setup() override;
    virtual void render() override;
//    virtual void releaseRes() override;
    
    ~StencilTestRender(){}
private:
    Shader *_shaderSingleColor = nullptr;
};


#endif /* StencilTest_hpp */

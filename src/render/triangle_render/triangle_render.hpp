//
//  triangleRender.hpp
//  LearnOpenGL
//
//  Created by ONEMC on 2023/7/4.
//

#ifndef triangleRender_hpp
#define triangleRender_hpp

#include "render.hpp"

class TriangleRender: public Render {
    
public:
    void setup() override;
    void render() override;
    
protected:
    unsigned int compileVertexShader(const char *program);
    unsigned int compileFragmentShader(const char *program);
    unsigned int linkProgram(unsigned int,  unsigned int);
    unsigned int shaderProgram=0, vertexShader=0, fragmentShader=0;
    virtual void setupVertex();
};

class TriangleRender_EBO: public TriangleRender {
public:
    void render() override;
    void setupVertex() override;
};
#endif /* triangleRender_hpp */

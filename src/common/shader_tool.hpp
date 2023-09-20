//
//  shader_tool.hpp
//  LearnOpenGL
//
//  Created by MC on 2023/7/8.
//

#ifndef shader_tool_hpp
#define shader_tool_hpp

extern unsigned int SCREEN_PIXEL_WIDTH;
extern unsigned int SCREEN_PIXEL_HEIGHT;

unsigned int setupCubeVertex();
unsigned int setupCubeVertexNormal();
unsigned int setupCubeVertexMSAA();
unsigned int setupPlaneVertex();
unsigned int setupPlaneVertexNomal(float* planeVertices, int size);
unsigned int setupQuad();
unsigned int setupSkyBox();
unsigned int setupFourVertex();

void renderCube(unsigned int& cubeVAO);
void renderQuad(unsigned int& quadVAO);
void renderQuadMapping(unsigned int& quadVAO);


#endif /* shader_tool_hpp */

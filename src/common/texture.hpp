//
//  texture.hpp
//  LearnOpenGL
//
//  Created by MC on 2023/7/6.
//

#ifndef texture_hpp
#define texture_hpp

#include <iostream>
#include <glad/glad.h>

using namespace std;

unsigned int load2DTexture(std::string path,
                           bool flip_y = true,
                           unsigned int warpping = GL_REPEAT,
                           bool gamma = false);

unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);

// loads a cubemap texture from 6 individual texture faces
// order:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front)
// -Z (back)
unsigned int loadCubemap(std::vector<std::string> faces, bool flip_y = false);

unsigned int loadHdr(std::string path);

#endif /* texture_hpp */

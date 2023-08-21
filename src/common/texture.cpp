//
//  texture.cpp
//  LearnOpenGL
//
//  Created by MC on 2023/7/6.
//

#include "texture.hpp"
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

unsigned int load2DTexture(std::string path, bool flip_y, unsigned int warpping, bool gamma) {
    
    stbi_set_flip_vertically_on_load(flip_y);
    
    int width, height, nrComponents;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
    if (!data) {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
        assert(false);
        return 0;
    }
    
    GLenum internalFormat = GL_RGB;
    GLenum dataFormat = GL_RGB;
    if (nrComponents == 1){
        internalFormat = dataFormat = GL_RED;
    } else if (nrComponents == 3) {
        internalFormat = gamma ? GL_SRGB : GL_RGB;
        dataFormat = GL_RGB;
    } else if (nrComponents == 4) {
        internalFormat = gamma ? GL_SRGB_ALPHA : GL_RGBA;
        dataFormat = GL_RGBA;
    }
    
    unsigned int textureID;
    glGenTextures(1, &textureID); // create
    
    glBindTexture(GL_TEXTURE_2D, textureID); // init
    
    /*
     set the texture wrapping/filtering options (on the currently bound texture object)
     */
    
    /*
     Copy image to GPU data
     params:
     1: GL_TEXTURE_1D / GL_TEXTURE_2D / GL_TEXTURE_3D
     2: set mipmap level: 0 base level
     3: texture stored format: GL_RGB-> rgb only
     4&5: width & height
     6: always 0
     7&8: image: format & datatype
     9: image data
     */
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
    
    /*
     Texture wrap: Process coordinates beyond 0-1
     Key:
        - GL_TEXTURE_WRAP_S x
        - GL_TEXTURE_WRAP_T y
        - GL_TEXTURE_WRAP_R z
     Value:
        - GL_REPEAT
        - GL_MIRRORED_REPEAT
        - GL_CLAMP_TO_EDGE
        - GL_CLAMP_TO_BORDER
     */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, dataFormat == GL_RGBA ? warpping : GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, dataFormat == GL_RGBA ? warpping : GL_REPEAT);
    

    /*
    // Only for `GL_CLAMP_TO_BORDER`
    set border color
    float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
     */
    
    /*
     Texture filter: Processing texture resolution is too low
     Key:
        - GL_TEXTURE_MAG_FILTER
     Value:
        - GL_NEAREST
        - GL_LINEAR
     */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    /*
     Texture filter: Processing texture resolution is too high
     Key:
        - GL_TEXTURE_MIN_FILTER
     Value:
      - GL_NEAREST_MIPMAP_NEAREST
      - GL_LINEAR_MIPMAP_NEAREST
      - GL_NEAREST_MIPMAP_LINEAR
      - GL_LINEAR_MIPMAP_LINEAR
     */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    
    return textureID;
}

unsigned int TextureFromFile(const char *path, const string &directory, bool gamma) {
    string filename = string(path);
    filename = directory + '/' + filename;
    return load2DTexture(filename);
}

// loads a cubemap texture from 6 individual texture faces
// order:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front)
// -Z (back)
// -------------------------------------------------------
unsigned int loadCubemap(std::vector<std::string> faces, bool flip_y) {
    stbi_set_flip_vertically_on_load(flip_y);
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}


unsigned int loadHdr(std::string path) {
    // pbr: load the HDR environment map
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrComponents;
    float *data = stbi_loadf(path.c_str(), &width, &height, &nrComponents, 0);
    unsigned int hdrTexture = 0;
    if (data) {
        glGenTextures(1, &hdrTexture);
        glBindTexture(GL_TEXTURE_2D, hdrTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data); // note how we specify the texture's data value to be float

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else {
        std::cout << "Failed to load HDR image." << std::endl;
    }
    return hdrTexture;
}


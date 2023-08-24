//
//  Debug.hpp
//  LearnOpenGL
//
//  Created by ONEMC on 2023/5/19.
//

#ifndef DebugOGL_hpp
#define DebugOGL_hpp

#include <glad/glad.h>

GLenum glCheckError_(const char *file, int line);
#define glCheckError() glCheckError_(__FILE__, __LINE__)

// 'glDebugMessageCallback' is supported in OpenGL 4.3
// https://registry.khronos.org/OpenGL-Refpages/gl4/html/glDebugMessageCallback.xhtml
void openglDebugMode();
void APIENTRY glDebugOutput(GLenum source,
                            GLenum type,
                            unsigned int id,
                            GLenum severity,
                            GLsizei length,
                            const char *message,
                            const void *userParam);

#endif /* Debug_hpp */
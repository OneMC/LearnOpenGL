#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

uniform mat4 transform; // 接收transform matrix

void main()
{
    // 应用线性变换矩阵
    gl_Position = transform * vec4(aPos, 1.0);
    ourColor = aColor;
    TexCoord = aTexCoord;
}

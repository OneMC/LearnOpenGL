#version 330 core
out vec4 FragColor;

float near = 0.1;
float far  = 100.0;

in vec2 TexCoords;
uniform sampler2D texture1;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // back to NDC
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{
    // If the 'texture1' is not used in the program,
    // it will be optimized and 'setint' will report error
    FragColor = texture(texture1, TexCoords); // texutre color
//    FragColor = vec4(vec3(gl_FragCoord.z), 1.0); // linear color
    float depth = LinearizeDepth(gl_FragCoord.z) / far; // divide by far for demonstration
    FragColor = vec4(vec3(depth), 1.0);
}

#version 330 core
out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
//    FragColor = texture(texture1, TexCoord);

    //    FragColor = texture(texture1, TexCoord) * vec4(ourColor, 1.0);
    
    // 0.8 * texture1.color + 0.2*texture2.color
    FragColor = mix(texture(texture1, TexCoord),
                    texture(texture2, TexCoord), 0.2);

}

#version 330 core
in vec2 TexCoord;

out vec4 FragColor;

// out vec4 frag_color;
//
// uniform sampler2D ourTexture;
//
// void main()
// {
//     frag_color = texture(ourTexture, TexCoord);
// }

void main()
{
    FragColor = vec4(0.0f, 0.5f, 0.2f, 1.0f);
} 

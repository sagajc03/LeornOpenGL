#version 460 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

void main()
{
    // DEBUG: visualiza las UVs (x->rojo, y->verde)
    FragColor = vec4(TexCoord, 0.0, 1.0);
}
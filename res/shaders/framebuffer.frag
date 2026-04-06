#version 330 core
in vec2 v_tex_coord;

out vec4 f_colour;

uniform sampler2D frame_texture;

void main()
{
    f_colour = texture(frame_texture, v_tex_coord);
}

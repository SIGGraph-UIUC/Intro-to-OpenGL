#version 330

in vec2 vTexCoord;

uniform sampler2D color_texture;

out vec4 fragColor;

void main()
{
  fragColor = texture(color_texture, vTexCoord);
}

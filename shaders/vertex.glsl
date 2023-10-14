#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 vWorldPos;
out vec2 vTexCoord;

void main()
{
  vec4 world_space = model * vec4(position, 1.0f);
  gl_Position = projection * view * world_space;

  vWorldPos = world_space.xyz;
  vTexCoord = uv;
}

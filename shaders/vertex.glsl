#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 vWorldPos;
out vec3 vWorldNormal;
out vec2 vTexCoord;

void main()
{
  vec4 world_pos = model * vec4(position, 1.0f);
  vec4 world_normal = model * vec4(normal, 0.0f);
  gl_Position = projection * view * world_pos;

  vWorldPos = world_pos.xyz;
  vWorldNormal = world_normal.xyz;
  vTexCoord = uv;
}

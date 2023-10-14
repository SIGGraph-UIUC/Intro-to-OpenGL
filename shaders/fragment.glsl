#version 330

uniform mat4 model;

in vec3 vWorldPos;
in vec2 vTexCoord;

uniform sampler2D diffuse_texture;
uniform sampler2D normal_texture;

layout (location = 0) out vec4 gWorldPos;
layout (location = 1) out vec4 gWorldNormal;
layout (location = 2) out vec4 gDiffuseSpecular;

void main()
{
  gWorldNormal = model * vec4(texture(normal_texture, vTexCoord).xyz, 0.0f);
  gWorldPos = vec4(vWorldPos, 1.0f);

  const float specular = 0.5f;
  gDiffuseSpecular = vec4(texture(diffuse_texture, vTexCoord).xyz, specular);
}

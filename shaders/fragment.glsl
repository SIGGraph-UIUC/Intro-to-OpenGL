#version 330

uniform mat4 model;

in vec3 vWorldPos;
in vec3 vWorldNormal;
in vec2 vTexCoord;

uniform sampler2D diffuse_texture;

layout (location = 0) out vec4 gWorldPos;
layout (location = 1) out vec4 gWorldNormal;
layout (location = 2) out vec4 gDiffuseSpecular;

void main()
{
  gWorldNormal.xyz = vec3(vWorldNormal);
  gWorldPos = vec4(vWorldPos, 1.0f);

  const float specular = 0.5f;
  gDiffuseSpecular = vec4(texture(diffuse_texture, vTexCoord).xyz, specular);
}

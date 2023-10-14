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
  gWorldNormal = normalize(model * vec4(2 * texture(normal_texture, vTexCoord).xyz - 1.0f, 0.0f));
  gWorldPos = vec4(vWorldPos, 1.0f);

  const float specular = 1.0f;
  gDiffuseSpecular = vec4(texture(diffuse_texture, vTexCoord).xyz, specular);
}

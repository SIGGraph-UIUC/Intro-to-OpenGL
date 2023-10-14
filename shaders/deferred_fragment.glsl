#version 330

uniform vec3 viewPos;

uniform sampler2D gWorldPos;
uniform sampler2D gWorldNormal;
uniform sampler2D gDiffuseSpecular;

in vec2 vTexCoord;

const vec3 light_pos = vec3(0.0f, 3.0f, 3.0f);
const vec3 light_color = vec3(1.0f, 1.0f, 1.0f);
const float light_intensity = 100.0f;

out vec4 fragColor;

void main()
{
    vec4 position = texture(gWorldPos, vTexCoord);
    vec3 normal = texture(gWorldNormal, vTexCoord).rgb;
    vec4 diff_spec = texture(gDiffuseSpecular, vTexCoord);

    // If the normal has length 0, we can assume that
    // there's not an actual object there.
    if (length(normal) == 0.0) discard;

    // Otherwise proceed with shading
    vec3 world_pos = position.xyz;
    vec3 diffuse = diff_spec.rgb;
    float specular = diff_spec.a;
    vec3 view_dir = normalize(viewPos - world_pos);
    normal = normalize(normal);

    // Add up the contributions from the light
    vec3 light_dir = normalize(light_pos - world_pos);
    float light_dist = length(light_pos - world_pos);

    // Ambient component
    float ambient_value = 0.1f;
    vec3 ambient_color = light_color * diffuse * ambient_value;

    // Diffuse component
    float diffuse_value = max(dot(normal, light_dir), 0.0f);
    vec3 diffuse_color = diffuse_value * diffuse * light_color;

    // Specular component
    vec3 half_dir = normalize(light_dir + view_dir);
    float specular_value = pow(max(dot(normal, half_dir), 0.0f), 256.0f);
    vec3 specular_color = specular_value * specular * light_color;

    // Add components with attenuation
    float attenuation = 1.0f / (1.0f + 0.5f * light_dist);
    vec3 color = (diffuse_color + specular_color) * attenuation + ambient_color;

    fragColor = vec4(color, 1.0f);
}
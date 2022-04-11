#version 330

in vec2 texcoord;
in float light;

uniform sampler2D u_texture_0;

uniform vec3 light_position;
uniform vec3 eye_position;
uniform vec3 light_direction;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform int is_gun;

in vec3 world_position;
in vec3 Normal;

void main()
{
    vec4 light_color = vec4(1);

    float ambient = 0.009;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light_position - world_position);
    float diff = max(dot(norm, lightDir), 0);
    float diffuse = material_kd * diff;

    vec3 viewDir = normalize(eye_position - world_position);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material_shininess);
    float specular = material_ks * 0.01 * spec * max(dot(norm, lightDir), 0);

    float distance = distance(eye_position, world_position);
    float attenuation;

    float theta = dot(lightDir, normalize(-light_direction));
    float light_cutoff = cos(radians(25.f));
    float light_outer_cutoff = cos(radians(22.f));
    float epsilon = light_cutoff - light_outer_cutoff;
    float intensity = smoothstep(0.0, 1.0, (light_outer_cutoff - theta) / epsilon);

    if (theta > light_cutoff) {
        attenuation = 1 / (1 + 0.14 * distance + 0.01 * distance * distance);
    } else {
        attenuation = 0;
    }

    diffuse *= intensity;
    specular *= intensity;

    gl_FragColor = texture2D(u_texture_0, texcoord) * light_color * (ambient + attenuation * (diffuse + specular));
}

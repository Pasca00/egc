#version 330

// Input
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position;
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

// TODO(student): Declare any other uniforms

uniform bool isSpotlight;
uniform float cut_off;

uniform vec3 object_color;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    vec3 intensity = vec3(1, 1, 1);
    // TODO(student): Define ambient, diffuse and specular light components
    float ambient_light = 0.25;
    float diffuse_light = 0;
    float specular_light = 0;

    vec3 L = normalize(light_position - world_position);
    vec3 V = normalize(eye_position - world_position);
    vec3 H = normalize(L + V);
    vec3 R = normalize(reflect(L, world_normal));

    ambient_light = 0.25 * material_ks;
    diffuse_light = material_kd * 1 * max(dot(world_normal, L), 0);
    // It's important to distinguish between "reflection model" and
    // "shading method". In this shader, we are experimenting with the Phong
    // (1975) and Blinn-Phong (1977) reflection models, and we are using the
    // Phong (1975) shading method. Don't mix them up!
    if (diffuse_light > 0)
    {
        specular_light = material_ks * max(dot(world_normal, L), 0) * pow(max(dot(world_normal, H), 0), material_shininess);
    }

    // TODO(student): If (and only if) the light is a spotlight, we need to do
    // some additional things.
    float light = 0.f;
    if (isSpotlight) {
        float cutOff = radians(cut_off);
        float spot_light = dot(-L, light_direction);

        if (spot_light > cos(cutOff)) {
            float attenuation_factor = 1 / pow(distance(light_position, world_position), 2);
            float spot_light_limit = cos(cutOff);
            float linear_att = (spot_light - spot_light_limit) / (1.0f - spot_light_limit);
            float light_att_factor = pow(linear_att, 2);

            light = ambient_light + light_att_factor * (diffuse_light + specular_light);
        } else {
            light = ambient_light;
        }
    } else {
        float attenuation_factor = 1 / pow(distance(light_position, world_position), 2);
        light = ambient_light + attenuation_factor * (diffuse_light + specular_light);
        // TODO(student): Write pixel out color
    }
    out_color = vec4(object_color, 1) * light;

}

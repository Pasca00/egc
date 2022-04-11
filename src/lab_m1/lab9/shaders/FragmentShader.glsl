#version 330

// Input
in vec2 texcoord;

// Uniform properties
uniform sampler2D texture_1;
uniform sampler2D texture_2;
// TODO(student): Declare various other uniforms
uniform int mix_required;
uniform int is_globe;
uniform float time;
// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // TODO(student): Calculate the out_color using the texture2D() function.
    vec2 t_coord = texcoord;
    if (is_globe == 1) {
        t_coord = texcoord + vec2(time, 0);
    }

    vec4 c1 = texture2D(texture_1, t_coord);
    vec4 c2 = texture2D(texture_2, t_coord);

    if (mix_required == 1) {
        out_color = mix(c1, c2, .5f);
    } else {
        out_color = c1;
    }

    if (out_color.a <= .5f) {
        discard;
    }
}

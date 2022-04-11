#version 330

// Input
// TODO(student): Get values from vertex shader
in float frag_color;
in vec3 frag_normal;
in float x;
// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // TODO(student): Write pixel out color
    out_color = vec4(
        cos(frag_color + x) + frag_normal.x, 
        (1 - sin(frag_color + x)) + frag_normal.y, 
        (1 - cos(frag_color + x)) * sin(frag_color + x) + frag_normal.y, 
        0);
}

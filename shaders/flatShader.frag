#version 450

in vec3 color;

out vec4 fragColor;

void main()
{

  fragColor = clamp(vec4(color, 1.0), vec4(0.0), vec4(1.0));
}

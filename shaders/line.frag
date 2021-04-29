#version 450

out vec4 fColor;

layout(std140, binding = 0) uniform constants {
  mat4 mvp;
  vec3 color;
};

void main()
{
  fColor = vec4(color, 1.0);
}
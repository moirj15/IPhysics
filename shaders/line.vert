#version 450

layout(location = 0) in vec3 vPosition;

layout(std140, binding = 0) uniform constants {
  mat4 mvp;
  vec3 color;
};

void main()
{
  gl_Position = mvp * vec4(vPosition, 1.0);
}

#version 450

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;

uniform mat4 projection = mat4(1.0);
uniform mat4 transform = mat4(1.0);
uniform mat4 camera = mat4(1.0);

uniform vec3 lightPosition = vec3(1.0);

out vec3 color;


void main()
{
  mat4 modelView = camera * transform;
  color = vPosition;


  gl_Position = projection * modelView * vec4(vPosition, 1.0);
}
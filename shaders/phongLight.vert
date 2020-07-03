#version 450

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec3 vOffset;


uniform mat4 camera;
uniform mat4 projModelView;
uniform mat3 normalMat;

uniform vec3 lightPosition;

out vec3 eye_vPosition;
out vec3 eye_LightPosition;
out vec3 eye_normal;

void main()
{
  vec4 position = vec4(vPosition + vOffset, 1.0);
  gl_Position = projModelView * position;
  eye_vPosition = vec3(camera * position);
  eye_LightPosition = vec3(camera * vec4(lightPosition, 1.0));
  eye_normal = normalMat * normalize(vNormal);

}

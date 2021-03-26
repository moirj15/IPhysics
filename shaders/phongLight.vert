#version 450

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
//layout (location = 2) in vec3 vOffset;


layout(std140, binding = 0) uniform constants {
  mat4 camera;
  mat4 mvp;
  mat4 normalMat;
};

uniform vec3 lightPosition;

out vec3 eye_vPosition;
out vec3 eye_LightPosition;
out vec3 eye_normal;

void main()
{
  vec4 position = vec4(vPosition, 1.0);//vec4(vPosition + vOffset, 1.0);
  gl_Position = mvp * position;
  eye_vPosition = vec3(camera * position);
  eye_LightPosition = vec3(camera * vec4(lightPosition, 1.0));
  // TODO: might need 1.0 as the w component
  eye_normal = vec3(normalMat * normalize(vec4(vNormal, 0.0)));

}

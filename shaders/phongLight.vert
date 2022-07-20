#version 450

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
//layout (location = 2) in vec3 vOffset;


layout (location = 0) uniform mat4 camera;
layout (location = 1) uniform mat4 mvp;
layout (location = 2) uniform mat4 normalMat;
layout (location = 3) uniform vec4 lightPosition;


out vec3 eye_vPosition;
out vec3 eye_LightPosition;
out vec3 eye_normal;

void main()
{
  vec4 position = vec4(vPosition, 1.0);//vec4(vPosition + vOffset, 1.0);
  gl_Position = mvp * position;
  eye_vPosition = vec3(camera * position);
  eye_LightPosition = vec3(camera * lightPosition);
  // TODO: might need 1.0 as the w component
  eye_normal = vec3(normalMat * normalize(vec4(vNormal, 1.0)));

}

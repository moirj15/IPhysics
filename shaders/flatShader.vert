#version 450

// Modifed Phong shader so there's no interpolation

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;

uniform mat4 projection = mat4(1.0);
uniform mat4 transform = mat4(1.0);
uniform mat4 camera = mat4(1.0);

uniform vec3 lightPosition = vec3(1.0);

out vec3 color;


void main()
{
  const vec3 lightColor = vec3(1.0, 1.0, 1.0);
  const vec3 ambientLight = vec3(0.3, 0.3, 0.3);
  const vec3 diffuseColor = vec3(0.7, 0.8, 0.0);
  const vec3 specularColor = vec3(1.0);
  
  mat4 modelView = camera * transform;

  vec3 lightPositionEyeSpace = vec3(camera * vec4(lightPosition, 1.0));
  vec3 vPosEyeSpace = vec3(modelView * vec4(vPosition, 1.0));
  vec3 lightEyeSpace = vec3(modelView * vec4(lightPosition, 1.0));

  mat3 normalTransform = inverse(transpose(mat3(modelView)));
  vec3 N = normalize(normalTransform * vNormal);

  vec3 L = normalize(lightPositionEyeSpace - vPosEyeSpace);

  vec3 R = normalize( reflect(-L, N) );
  vec3 V = L;

  const vec3 ambientColor = vec3(0.3, 0.0, 0.0);
	// calculate components in the vertex shader so it looks flat
  vec3 ambient = ambientColor * ambientLight;
  vec3 diffuse = lightColor * diffuseColor * max(dot(N,L),0.0);
  float specDot = pow( max(dot(R,V),0.0), /*specExp*/ 1.0 );
  vec3 specular = lightColor.xyz * specularColor * vec3(1.0, 1.0, 1.0) * specDot;

  color = (0.3 * ambient) + (1.0 * diffuse);// + (0.5 * specular); 

  gl_Position = projection * modelView * vec4(vPosition, 1.0);
}
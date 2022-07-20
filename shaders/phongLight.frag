#version 450

layout (location = 4) uniform vec4 lightColor;
layout (location = 5) uniform vec4 ambientLight;
layout (location = 6) uniform vec4 ambientColor;
layout (location = 7) uniform vec4 diffuseColor;
layout (location = 8) uniform vec4 specularColor;
 // x: ambient coeff
 // y: diffuse coeff
 // z: specular coeff
 // w: specular exp
layout (location = 9) uniform vec4 coeff;



in vec3 eye_vPosition;
in vec3 eye_LightPosition;
in vec3 eye_normal;

out vec4 fragmentColor;

void main()
{
  vec3 L = normalize(eye_LightPosition - eye_vPosition);
  vec3 N = normalize(eye_normal);
  vec3 R = normalize(reflect(-L, N));
  vec3 V = normalize(-eye_vPosition); // may need to be camera position

  vec3 ambient = ambientColor.rgb * ambientLight.rgb * coeff.x;

  vec3 diffuse = diffuseColor.rgb * lightColor.rgb * max(dot(N, L), 0.0) * coeff.y;

  float specDot = pow(max(dot(R, V), 0.0), coeff.w);
  vec3 specular = specularColor.rgb * lightColor.rgb * specDot * coeff.z;

  vec4 color = vec4(ambient + diffuse + specular, 1.0);

  fragmentColor = clamp(color, vec4(0.0), vec4(1.0));
//  fragmentColor = vec4(1.0);
}

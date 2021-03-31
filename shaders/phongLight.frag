#version 450

uniform vec3 lightColor;
uniform vec3 ambientLight;

uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;


// x: ambient coeff
// y: diffuse coeff
// z: specular coeff
// w: specular exp
uniform vec4 coeff;

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

  vec3 ambient = ambientColor * ambientLight * coeff.x;

  vec3 diffuse = diffuseColor * lightColor * max(dot(N, L), 0.0) * coeff.y;

  float specDot = pow(max(dot(R, V), 0.0), coeff.w);
  vec3 specular = specularColor * lightColor * specDot * coeff.z;

  vec4 color = vec4(ambient + diffuse + specular, 1.0);

  fragmentColor = clamp(color, vec4(0.0), vec4(1.0));
  fragmentColor = vec4(1.0);
}

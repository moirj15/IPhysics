#version 450

layout (location = 0) in vec3 vPosition;

uniform mat4 projection = mat4(1.0);
uniform mat4 transform = mat4(1.0);
uniform mat4 camera = mat4(1.0);

out vec4 position;

void main()
{
	gl_PointSize = 10.0;
	gl_Position = projection * camera * transform * vec4(vPosition, 1.0);
}

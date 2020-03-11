#version 450

layout (location = 0) in vec3 vPosition;

uniform mat4 projection = mat4(1.0);
uniform mat4 transform = mat4(1.0);
uniform mat4 camera = mat4(1.0);
uniform mat4 projModelView;
void main()
{
	gl_Position = projection * camera * transform * vec4(vPosition, 1.0);
}
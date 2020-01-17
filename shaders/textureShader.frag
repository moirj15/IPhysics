#version 450

uniform vec3 color = vec3(1.0);
out vec4 fragmentColor;

void main()
{
	fragmentColor = vec4(color, 1.0);
}

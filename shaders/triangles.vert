#version 330 core

uniform mat4 camera_r;
uniform mat4 camera_t;
uniform mat4 projection;
uniform mat4 obj_t;

in vec4 vertex;
in vec4 vColour;

out vec4 fColour;

void main()
{
	fColour = vColour;

	gl_Position = projection*camera_r*camera_t*obj_t*vertex;
}

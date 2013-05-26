#version 330 core

uniform mat4 project_view_model;

in  vec4 vertex;
in  vec4 vColour;

out vec4 fColour;

void main()
{
	fColour = vColour;

	gl_Position = project_view_model*vertex;
}

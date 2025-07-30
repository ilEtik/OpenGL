#shader vertex
#version 330 core

layout(location = 0) in vec4 _position;

void main()
{
	gl_Position = _position;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 _color;

uniform vec4 u_Color;

void main()
{
	_color = u_Color;
}
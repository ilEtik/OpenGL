#shader vertex
#version 330 core

layout(location = 0) in vec4 _position;

out vec4 v_Position;

void main()
{
	v_Position = _position;
	gl_Position = _position;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 _color;

in vec4 v_Position;

void main()
{
	_color = v_Position + 0.5;
}
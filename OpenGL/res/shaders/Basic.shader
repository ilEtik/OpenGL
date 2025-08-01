#shader vertex
#version 330 core

layout(location = 0) in vec4 _position;
layout(location = 1) in vec2 _texCoord;

out vec2 v_TexCoord;

uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * _position;
	v_TexCoord = _texCoord;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 _color;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;

void main()
{
	vec4 texColor = texture(u_Texture, v_TexCoord);
	_color = texColor;
}
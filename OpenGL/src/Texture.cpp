#include "Texture.h"

#include "stb_image/stb_image.h"

Texture::Texture(const std::string& path)
	: _rendererId(0), _filePath(path), _localBuffer(nullptr), _width(0), _height(0), _bpp(0)
{
	stbi_set_flip_vertically_on_load(1);
	_localBuffer = stbi_load(path.c_str(), &_width, &_height, &_bpp, 4);

	GLCall(glGenTextures(1, &_rendererId));
	GLCall(glBindTexture(GL_TEXTURE_2D, _rendererId));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _localBuffer));
	Unbind();

	if (_localBuffer)
	{
		stbi_image_free(_localBuffer);
	}
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &_rendererId));
}

void Texture::Bind(unsigned int slot /*= 0*/) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, _rendererId));
}

void Texture::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
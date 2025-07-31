#pragma once

#include "Renderer.h"

class Texture
{
public:
	Texture(const std::string& path);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int GetWidth() const { return _width; }
	inline int GetHeight() const { return _height; }

private:
	unsigned int _rendererId;
	std::string _filePath;
	unsigned char* _localBuffer;
	int _width, _height, _bpp;
};
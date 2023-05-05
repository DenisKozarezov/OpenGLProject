#pragma once
#include <pch.h>

class Texture
{
private:
	GLuint ID;
	void loadTexture(const std::string& texturePath);
public:
	Texture() = delete;
	Texture(const std::string& texturePath);
	~Texture();
};


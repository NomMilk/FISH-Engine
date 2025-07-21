#include "Texture.h"
#include <stdexcept>
#include <iostream>

Texture::Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixleType)
{
	type = texType;
	path = std::string(image);
	
	int widthImg, heightImg, numColCh;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);

	if (!bytes) {
		throw std::runtime_error("Failed to load texture: " + std::string(image) + " - " + std::string(stbi_failure_reason()));
	}

	std::cout << "Loaded texture: " << image << " (" << widthImg << "x" << heightImg << ", " << numColCh << " channels)" << std::endl;

	glGenTextures(1, &ID);
	glActiveTexture(slot);
	glBindTexture(texType, ID);

	glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// check for correct format and set internal format based on number of channels
	GLint internalFormat;
	if (format == GL_RGB)
		internalFormat = GL_RGB;
	else if (format == GL_RGBA)
		internalFormat = GL_RGBA;
	else
		internalFormat = GL_RGB;

	glTexImage2D(texType, 0, internalFormat, widthImg, heightImg, 0, format, pixleType, bytes);
	glGenerateMipmap(texType);

	stbi_image_free(bytes);
	glBindTexture(texType, 0);
}

void Texture::texUnit(Shader shader, const char* uniform, GLuint unit)
{
	GLuint tex0Uni = glGetUniformLocation(shader.ID, "tex0");
	shader.Activate();
	glUniform1i(tex0Uni, 0);
}

void Texture::Bind()
{
	glBindTexture(type, ID);
}

void Texture::Unbind()
{
	glBindTexture(type, 0);
}

void Texture::Delete()
{
	glDeleteTextures(1, &ID);
}
#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include <glad/glad.h>
#include <stb/stb_image.h>
#include <string>

#include "shaderClass.h"

class Texture
{
	public:
		GLuint ID;
		GLenum type;
		std::string path;
		Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixleType);

		void texUnit(Shader shader, const char* uniform, GLuint unit);
		void Bind();
		void Unbind();
		void Delete();
};

#endif

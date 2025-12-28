#ifndef TEXT_LOADER_H
#define TEXT_LOADER_H

#include <string>
#include <vector>
#include "shaderClass.h"
#include "Texture.h"

struct TextInfo {
	std::string text;
	//posiiton
	float x, y;
};

class TextLoader {
public:
    TextLoader();
    ~TextLoader();
	//honestly might be an issue with dangling ptrs but like ahhh should be fineeeeeeee
    TextInfo* drawText(Shader& shader, std::string text, float x, float y);

private:
	TextInfo currentText[10] = {};
	unsigned int currentIndex = 0;
};

#endif

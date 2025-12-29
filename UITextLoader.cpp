#include "UITextLoader.h"
#include "Texture.h"
#include <stdexcept>

TextLoader::TextLoader() = default;
TextLoader::~TextLoader() = default;

TextInfo* TextLoader::drawText(Shader& shader, std::string text, float x, float y)
{
    //currentIndex += 1;
    if (currentIndex > 9)
        throw std::runtime_error("This engine only supports less than 10 text bro");

    TextInfo editedText;
    editedText.text = text;
    editedText.x = x;
    editedText.y = y;
    currentText[currentIndex] = editedText;

    //Texture testTexture("goldfish.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);

	shader.Activate();
    //testTexture.Bind();
    glUniform1i(glGetUniformLocation(shader.ID, "useTexture"), 1);

    glDisable(GL_DEPTH_TEST);

    float quadVertices[] = {
        // pos              // normal       // uv
        -0.5f, -0.5f, 0.0f,  0,0,1,          0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
         0.5f, -0.5f, 0.0f,  0,0,1,          1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
         0.5f,  0.5f, 0.0f,  0,0,1,          1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f,  0,0,1,          0.0f, 1.0f, 1.0f, 1.0f, 1.0f
    };

    unsigned int quadIndices[] = {
        0, 1, 2,
        2, 3, 0
    };

    GLuint quadVBO, quadEBO;
    glGenBuffers(1, &quadVBO);
    glGenBuffers(1, &quadEBO);

    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glEnable(GL_DEPTH_TEST);
    //testTexture.Unbind();

    glDeleteBuffers(1, &quadVBO);
    glDeleteBuffers(1, &quadEBO);

    return &currentText[currentIndex];
}


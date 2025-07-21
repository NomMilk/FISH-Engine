#version 330 core
out vec4 FragColor;

in vec3 vertexColor;
in vec2 texCoords;

uniform sampler2D tex0;
uniform bool useTexture;

void main()
{
    if (useTexture) {
        vec4 texColor = texture(tex0, texCoords);
        if(texColor.a < 0.1)
            discard;
        FragColor = texColor;
    }
    else
        FragColor = vec4(vertexColor, 1.0);
}

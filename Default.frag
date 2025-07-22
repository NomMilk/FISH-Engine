#version 120

varying vec3 vertexColor;
varying vec2 texCoords;

uniform sampler2D tex0;
uniform bool useTexture;

void main()
{
    if (useTexture) {
        vec4 texColor = texture2D(tex0, texCoords);
        if(texColor.a < 0.1)
            discard;
        gl_FragColor = texColor;
    }
    else
        gl_FragColor = vec4(vertexColor, 1.0);
}

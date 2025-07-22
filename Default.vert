#version 120

attribute vec3 aPos;
attribute vec3 aColor;
attribute vec2 aTex;

varying vec3 vertexColor;
varying vec2 texCoords;

uniform mat4 camMatrix;
uniform mat4 model;

void main()
{
    gl_Position = camMatrix * model * vec4(aPos, 1.0);
    vertexColor = aColor;
    texCoords = aTex;
}

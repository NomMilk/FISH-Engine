#version 120

attribute vec3 aPos;
attribute vec3 aColor;
attribute vec2 aTex;
attribute vec3 aNormal;

varying vec3 vertexColor;
varying vec2 texCoords;
varying vec3 normal;

uniform mat4 camMatrix;
uniform mat4 model;
uniform mat3 normalMatrix;

void main()
{
    gl_Position = camMatrix * model * vec4(aPos, 1.0);
    
    vertexColor = aColor;
    texCoords = aTex;

    // transform and normalize the normal
    normal = normalize(normalMatrix * aNormal);
}

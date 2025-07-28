#version 120

uniform sampler2D tex0;
uniform bool useTexture;

uniform vec4 lightColor;
uniform vec3 lightDir;     // Directional light, should be normalized

varying vec3 normal;       // From vertex shader, interpolated
varying vec2 texCoords;
varying vec3 vertexColor;

void main()
{
    vec3 N = normalize(normal);
    vec3 L = normalize(lightDir);

    // Lambertian diffuse term
    float diff = max(dot(N, L), 0.0);

    vec4 baseColor;

    if (useTexture) {
        vec4 texColor = texture2D(tex0, texCoords);
        if (texColor.a < 0.1)
            discard;
        baseColor = texColor;
    } else {
        baseColor = vec4(vertexColor, 1.0);
    }

    // Apply lighting
    vec4 diffuse = baseColor * lightColor * diff;

    // Add ambient to avoid full darkness
    float ambient = 0.2;
    vec4 ambientColor = baseColor * ambient;

    gl_FragColor = diffuse + ambientColor;
}


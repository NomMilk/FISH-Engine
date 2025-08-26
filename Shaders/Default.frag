#version 120
#define MAX_LIGHTS 2

uniform sampler2D tex0;
uniform bool useTexture;

uniform vec4 lightColor;
uniform vec3 lightPos[MAX_LIGHTS];

varying vec3 normal;       // From vertex shader, interpolated
varying vec2 texCoords;
varying vec3 vertexColor;
varying vec3 fragPos;      // Fragment position in world space

void main()
{
	// Get base color from texture or vertex color
	vec4 baseColor;
	if (useTexture) {
		vec4 texColor = texture2D(tex0, texCoords);
		if (texColor.a < 0.1)
			discard;
		baseColor = texColor;
	} else {
		baseColor = vec4(vertexColor, 1.0);
	}

	// Initialize final color with ambient component
	float ambientStrength = 0.1;
	vec4 finalColor = ambientStrength * baseColor;

	// Normalize the normal vector
	vec3 N = normalize(normal);

	// Calculate view direction
	vec3 viewDir = normalize(vec3(0.0, 0.0, 1.0) - fragPos);

	for (int i = 0; i < MAX_LIGHTS; i++)
	{
		vec3 L = normalize(lightPos[i] - fragPos);

		// Diffuse component
		float diff = max(dot(N, L), 0.0);

		// Calculate height-based attenuation
		float heightFactor = 1.0 - clamp(fragPos.y * 0.05, 0.0, 0.5);

		// Calculate distance-based attenuation
		float distance = length(fragPos);
		float attenuation = 1.0 / (1.0 + 0.25 * distance + 0.05 * distance * distance);

		// Combine attenuation factors
		float combinedAttenuation = attenuation * (1.0 + heightFactor);

		// Accumulate diffuse lighting
		finalColor += diff * lightColor * baseColor * combinedAttenuation;

		// Specular component
		float specularStrength = 0.2;
		float shininess = 10.0;
		vec3 halfwayDir = normalize(L + viewDir);
		float spec = pow(max(dot(N, halfwayDir), 0.0), shininess);
		
		// Accumulate specular lighting
		finalColor += specularStrength * spec * lightColor * attenuation * heightFactor;
	}

	finalColor.a = baseColor.a; // Preserve original alpha
	gl_FragColor = finalColor;
}

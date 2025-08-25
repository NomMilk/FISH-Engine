#version 120
#define MAX_LIGHTS 2

uniform sampler2D tex0;
uniform bool useTexture;

uniform vec4 lightColor;
uniform vec3 lightPos[MAX_LIGHTS];     // Directional light, should be normalized

varying vec3 normal;       // From vertex shader, interpolated
varying vec2 texCoords;
varying vec3 vertexColor;
varying vec3 fragPos;      // Fragment position in world space

void main()
{
	// Normalize vectors
	vec3 N = normalize(normal);
	for (int I = 0; I < MAX_LIGHTS; I++)
	{
		vec3 L = normalize(lightPos[I] - fragPos);
		
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
		
		// Ambient component - increased for better base illumination
		float ambientStrength = 0.1;
		vec4 ambient = ambientStrength * baseColor;
		
		// Diffuse component with improved attenuation
		float diff = max(dot(N, L), 0.0);
		
		// Calculate height-based attenuation to avoid overly bright tops
		float heightFactor = 1.0 - clamp(fragPos.y * 0.05, 0.0, 0.5); // Reduce intensity as y increases
		
		// Calculate distance-based attenuation for more gradual falloff
		float distance = length(fragPos);
		float attenuation = 1.0 / (1.0 + 0.25 * distance + 0.05 * distance * distance);
		
		// Apply attenuation to diffuse lighting
		vec4 diffuse = diff * lightColor * baseColor * attenuation * (1.0 + heightFactor);
		
		// Specular component
		float specularStrength = 0.2;
		float shininess = 10.0; // Lower value = wider highlight
		// Calculate view direction - assume we're looking from camera position toward the fragment
		vec3 viewDir = normalize(vec3(0.0, 0.0, 1.0) - fragPos);
		vec3 halfwayDir = normalize(L + viewDir); // Blinn-Phong half-vector
		float spec = pow(max(dot(N, halfwayDir), 0.0), shininess);
		vec4 specular = specularStrength * spec * lightColor * attenuation * heightFactor;
		
		// Final color
		vec4 result = ambient + diffuse + specular;
		result.a = baseColor.a;  // Preserve original alpha
		
		gl_FragColor = result;
	}
}


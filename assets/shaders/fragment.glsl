#version 330 core
in vec3 color;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform sampler2D ourTexture;
uniform vec3 uLightPos;
uniform vec3 uViewPos;

void main() {
		vec3 norm = normalize(Normal);
		vec3 lightDir = normalize(uLightPos - FragPos);
		float diff = max(dot(norm, lightDir), 0.0);
		float ambient = 0.3;

		vec3 viewDir = normalize(uViewPos - FragPos);
		vec3 reflectDir = reflect(-lightDir, norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
		float specular = 0.5 * spec;

		float lighting = clamp(ambient + diff + specular, 0.0f, 1.0f);

    //vec4 texColor = texture(ourTexture, TexCoord) * vec4(Normal, 1.0f);
		vec4 texColor = vec4(color, 1.0f);
    FragColor = texColor * lighting;
		//FragColor = vec4(normalize(Normal) * 0.5 + 0.5, 1.0);

    //FragColor = texture(ourTexture, TexCoord) * vec4(Normal, 1.0f);
		//FragColor = vec4(color, 1.0f);
}

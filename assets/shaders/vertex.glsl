#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aNormal;

uniform mat4 uMVP;
uniform mat4 uModel;

out vec3 color;
out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

void main() {
    gl_Position = uMVP * vec4(aPos, 1.0);
		FragPos = vec3(uModel * vec4(aPos, 1.0));
		Normal = normalize(mat3(transpose(inverse(uModel))) * aNormal);
		color = aColor;
		TexCoord = aTexCoord;
}

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "FileLoader.h"

// Vertex Shader
const char* vertexShaderSource = R"(
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
)";

// Fragment Shader
const char* fragmentShaderSource = R"(
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
)";

int main(int argc, char* argv[]) {
		FileLoader fileloader("assets/models/utah_teapot.obj");
		
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

		// Initialize SDL Audio
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
				std::cerr << "SDL_mixer Error: " << Mix_GetError() << std::endl;
		}

		// Load Audio Files
		Mix_Chunk* jumpSound = Mix_LoadWAV("assets/audio/snd_bump.wav");
		Mix_Chunk* strangeSound = Mix_LoadWAV("assets/audio/snd_bluh.wav");
		if (!jumpSound) {
				std::cerr << "Failed to load jump sound: " << Mix_GetError() << std::endl;
		}
		if (!strangeSound) {
				std::cerr << "Failed to load strange sound: " << Mix_GetError() << std::endl;
		}

		// Initialize SDL Image
		int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
		if (!(IMG_Init(imgFlags) & imgFlags)) {
				std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
				SDL_Quit();
				return 1;
		}

    // Set OpenGL attributes
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // Create window
    SDL_Window* window = SDL_CreateWindow("OpenGL Moving Square",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1200, 900, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    
    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Create OpenGL context
    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        std::cerr << "SDL_GL_CreateContext Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW initialization failed!" << std::endl;
        return 1;
    }

    glViewport(0, 0, 1200, 900);
		glEnable(GL_DEPTH_TEST);

    // Compile shaders
		Shader shader(vertexShaderSource, fragmentShaderSource);

		// Load Texture
		Texture texture("assets/textures/obama.png");

		// Unused triangle object
		//float triangleVertices[] = {
		//		// positions          // colors           // texture coords   // normal
		//		 0.5f,  0.5f,  0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,      	1.0f, 1.0f, 1.0f,  // bottom left
		//		-0.5f,  0.5f, -1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,  			1.0f, 0.0f, 0.0f,  // bottom right
		//		-0.5f, -0.5f,  0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f,  			1.0f, 1.0f, 1.0f,  // top right
		//		 0.5f, -0.5f, -1.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,  			1.0f, 0.0f, 0.0f   // top left
		//};

    //unsigned int triangleIndices[] = {
    //    0, 1, 2,  // First triangle
		//		2, 3, 0   // Second triangle
    //};

		// Fetch Utah Teapot information from .obj file
		std::vector<float> teapotVertices = fileloader.getVertices();
		std::vector<unsigned int> teapotIndices = fileloader.getIndices();

		//Mesh triangle(triangleVertices, sizeof(triangleVertices), triangleIndices, sizeof(triangleIndices));
		Mesh teapot(teapotVertices.data(), teapotVertices.size() * sizeof(float), (unsigned int*)teapotIndices.data(), teapotIndices.size() * sizeof(unsigned int));

		// Initialize teapot rotation angle and speed
		float rotationAngle = 0.0f;
		float rotationSpeed = 1.0f;

		// Initialize camera object
		Camera camera(glm::vec3(0.0f, 3.0f, 3.0f));

		// Command to allow capturing mouse movements
		SDL_SetRelativeMouseMode(SDL_TRUE);

		// Administrative main loop variables
    bool running = true;
    SDL_Event e;
    const Uint8* keyState = SDL_GetKeyboardState(nullptr);

		// Initialize deltaTime variables
		Uint64 currentTime = SDL_GetPerformanceCounter();
    Uint64 lastTime = currentTime;
    double deltaTime = 0.0;
    
		// Crude physics sim variables
		bool jump = false;
		float tP = 1.0f;
		float tV = 0.0f;
		float tA = -10.0f;
		bool bounce = false;
		float oP = 1.0f;
		float oV = 0.0f;
		float oA = -10.0f;

    // Main loop
    while (running) {
				lastTime = currentTime;
        currentTime = SDL_GetPerformanceCounter();
        deltaTime = (double)((currentTime - lastTime) / (double)SDL_GetPerformanceFrequency());

        while (SDL_PollEvent(&e)) {
						switch (e.type) {
							case SDL_QUIT:
									running = false;
									break;
							case SDL_MOUSEMOTION:
									camera.rotate(e.motion.xrel, -e.motion.yrel);
									break;
							case SDL_KEYDOWN:
									if (e.key.keysym.sym == SDLK_SPACE) {
											tV = 5;
											jump = true;
									}
									break;
							default:
									break;
						}
        }

        // Handle arrow key input
				if (keyState[SDL_SCANCODE_W]) { camera.moveForward(deltaTime); }
				if (keyState[SDL_SCANCODE_S]) { camera.moveBackward(deltaTime); }
				if (keyState[SDL_SCANCODE_A]) { camera.moveLeft(deltaTime); }
				if (keyState[SDL_SCANCODE_D]) { camera.moveRight(deltaTime); }
        if (keyState[SDL_SCANCODE_LEFT])  { rotationAngle -= rotationSpeed; }
        if (keyState[SDL_SCANCODE_RIGHT]) { rotationAngle += rotationSpeed; }

        // Clear screen
        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use shader and set uniform
				shader.use();
				texture.bind(0);
				shader.setInt("ourTexture", 0);

				glm::vec3 lightPos = glm::vec3(5.0f * cos(0.05f), 5.0f, 5.0f * sin(0.05f));
				shader.setVec3("uLightPos", lightPos);

				shader.setVec3("uViewPos", camera.getPosition());

				// Create projection matrix (perspective)
				glm::mat4 projection = glm::perspective(
						glm::radians(70.0f),    // Field of view
						1200.0f / 900.0f,        // Aspect ratio
						0.1f,                   // Near plane
						100.0f                  // Far plane
				);

				// Create view matrix (move camera back so we can see the triangle)
				glm::mat4 view = camera.getViewMatrix();

				// Draw 10 triangles in a circle
				for (int i = 0; i < 10; i++) {
						float angle = (float)i / 10.0f * 360.0f;
						float x = cos(glm::radians(angle)) * 2.0f;
						float z = sin(glm::radians(angle)) * 2.0f;
						
						glm::mat4 model = glm::mat4(1.0f);
						model = glm::translate(model, glm::vec3(x, oP, z));
						model = glm::rotate(model, glm::radians(rotationAngle + angle), glm::vec3(0.0f, 1.0f, 0.0f));
						model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));  // Make them smaller
						
						glm::mat4 mvp = projection * view * model;
						shader.setMat4("uModel", model);
						shader.setMat4("uMVP", mvp);
						teapot.draw();
				}

				glm::mat4 model = glm::mat4(1.0f);
				// Apply jump physics to main teapot
				if(jump) {
						tV += tA * deltaTime;
						tP += tV * deltaTime;
						if(tP <= 0.0f) {
								// Play sound upon hitting ground
								Mix_PlayChannel(-1, jumpSound, 0);
								jump = false;
								// Cause smaller teapots to jump
								bounce = true;
								tP = 0.0f;
								// Transfer reduced amount of main teapot velocity to smaller teapots
								oV = -tV * 0.8;
						}
				}
				// Apply jump physics to smaller teapots
				if(bounce) {
						oV += oA * deltaTime;
						oP += oV * deltaTime;
						if(oP <= 0.0f) {
								// Play sound upon hitting ground
								Mix_PlayChannel(-1, jumpSound, 0);
								bounce = false;
								std::cout << std::abs(oV) << ' ' << (std::abs(oV) > 0.05f) << std::endl;
								// Cause main teapot to jump if smaller teapot velocity is over a minimum threshold
								if(std::abs(oV) > 1) {
										jump = true;
										// Transfer dampened amount of main teapot velocity to smaller teapots
										tV = -oV * 0.6;
								} else {
										// When the bouncing stops play a fun sound
										Mix_PlayChannel(-1, strangeSound, 0);
								}
								oP = 0.0f;
						}
				}

				// Translate, rotate, and scale main teapot
				model = glm::translate(model, glm::vec3(0.0f, tP, 0.0f));
				model = glm::rotate(model, glm::radians(rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));

				// Render the main teapot
				glm::mat4 mvp = projection * view * model;
				shader.setMat4("uModel", model);
				shader.setMat4("uMVP", mvp);
				teapot.draw();
				
				//Swap the SDL window
        SDL_GL_SwapWindow(window);
    }

		// Cleanup SDL
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
		IMG_Quit();
    SDL_Quit();

    return 0;
}

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <string>
#include "Camera.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "FileLoader.h"

const char* readFile(const std::string& path, std::string& storage) {
    std::ifstream file(path);

    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << path << std::endl;
        storage.clear();
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    storage = buffer.str();
    return storage.c_str();
}

glm::mat4 rotateAroundAxisThroughPoint(const glm::vec3& pointOnAxis,
                                       const glm::vec3& axis,
                                       float angleDegrees) {
    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, pointOnAxis);
    transform = glm::rotate(transform, glm::radians(angleDegrees), glm::normalize(axis));
    transform = glm::translate(transform, -pointOnAxis);
    return transform;
}

std::string vertexShaderStorage;
std::string fragmentShaderStorage;

// Vertex Shader
const char* vertexShaderSource = readFile("assets/shaders/vertex.glsl", vertexShaderStorage);
// Fragment Shader
const char* fragmentShaderSource = readFile("assets/shaders/fragment.glsl", fragmentShaderStorage);

int main(int argc, char* argv[]) {

  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  const int SCREEN_WIDTH = 1200;
  const int SCREEN_HEIGHT = 900;
  FileLoader teapotFile("assets/models/utah_teapot.obj");
  FileLoader monkeyFile("assets/models/blender_monkey.obj");

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
      SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

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

  glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
  glEnable(GL_DEPTH_TEST);

  // Compile shaders
  Shader shader(vertexShaderSource, fragmentShaderSource);

  // Load Texture
  Texture texture("assets/textures/bobby.jpg");

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
  std::vector<float> teapotVertices = teapotFile.getVertices();
  std::vector<unsigned int> teapotIndices = teapotFile.getIndices();

  std::vector<float> monkeyVertices = monkeyFile.getVertices();
  std::vector<unsigned int> monkeyIndices = monkeyFile.getIndices();

  //Mesh triangle(triangleVertices, sizeof(triangleVertices), triangleIndices, sizeof(triangleIndices));
  Mesh teapot(
      teapotVertices.data(),
      teapotVertices.size() * sizeof(float),
      (unsigned int*)teapotIndices.data(),
      teapotIndices.size() * sizeof(unsigned int)
  );

  Mesh monkey(
      monkeyVertices.data(),
      monkeyVertices.size() * sizeof(float),
      (unsigned int*)monkeyIndices.data(),
      monkeyIndices.size() * sizeof(unsigned int)
  );

  // Initialize teapot rotation angle and speed
  float rotationAngle = 0.0f;
  float rotationSpeed = 1.0f;

  // Initialize camera object
  Camera camera(glm::vec3(0.0f, 3.0f, 6.0f));

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

  bool animateObjects = false;
  float orbitAngle = 0.0f;
  float orbitSpeed = 50.0f;

  // Main loop
  while (running) {
    lastTime = currentTime;
    currentTime = SDL_GetPerformanceCounter();
    deltaTime = (double)((currentTime - lastTime) / (double)SDL_GetPerformanceFrequency());

    if (animateObjects) {
      orbitAngle += orbitSpeed * (float)deltaTime;
    }

    while (SDL_PollEvent(&e)) {
      switch (e.type) {
        case SDL_QUIT:
          running = false;
          break;
        case SDL_MOUSEMOTION:
          camera.rotate(e.motion.xrel, -e.motion.yrel);
          break;
        case SDL_KEYDOWN:
          if (e.key.keysym.sym == SDLK_r) {
            animateObjects = !animateObjects;
          }
          break;
        case SDL_MOUSEBUTTONDOWN:
          if (e.button.button == SDL_BUTTON_LEFT) {
            std::cout << "Left click at: " << e.button.x << ", " << e.button.y << std::endl;
          }
          if (e.button.button == SDL_BUTTON_RIGHT) {
            std::cout << "Right click!" << std::endl;
          }
          if (e.button.button == SDL_BUTTON_MIDDLE) {
            std::cout << "Middle click!" << std::endl;
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
        glm::radians(70.0f),
        (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
        0.1f,
        100.0f
    );

    // Create view matrix (move camera back so we can see the triangle)
    glm::mat4 view = camera.getViewMatrix();

    glm::vec3 teapotCenter(-1.5f, 0.0f, 0.0f);
    glm::vec3 monkeyCenter(1.5f, 0.0f, 0.0f);

    // Vertical axis halfway between the two objects
    glm::vec3 connectionAxis = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 axisPoint = (teapotCenter + monkeyCenter) * 0.5f;

    glm::mat4 axisRotation = rotateAroundAxisThroughPoint(axisPoint, connectionAxis, orbitAngle);

    glm::vec4 rotatedTeapotCenter4 = axisRotation * glm::vec4(teapotCenter, 1.0f);
    glm::vec4 rotatedMonkeyCenter4 = axisRotation * glm::vec4(monkeyCenter, 1.0f);

    glm::vec3 rotatedTeapotCenter = glm::vec3(rotatedTeapotCenter4);
    glm::vec3 rotatedMonkeyCenter = glm::vec3(rotatedMonkeyCenter4);

    // Translate, rotate, and scale main teapot
    glm::mat4 teapotModel = glm::mat4(1.0f);
    teapotModel = glm::translate(teapotModel, rotatedTeapotCenter);
    teapotModel = glm::rotate(teapotModel, glm::radians(rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    teapotModel = glm::scale(teapotModel, glm::vec3(0.5f, 0.5f, 0.5f));

    // Render the main teapot
    glm::mat4 teapotMVP = projection * view * teapotModel;
    shader.setMat4("uModel", teapotModel);
    shader.setMat4("uMVP", teapotMVP);
    teapot.draw();

    glm::mat4 monkeyModel = glm::mat4(1.0f);
    monkeyModel = glm::translate(monkeyModel, rotatedMonkeyCenter);
    monkeyModel = glm::rotate(monkeyModel, glm::radians(rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    monkeyModel = glm::scale(monkeyModel, glm::vec3(0.8f, 0.8f, 0.8f));

    glm::mat4 monkeyMVP = projection * view * monkeyModel;
    shader.setMat4("uModel", monkeyModel);
    shader.setMat4("uMVP", monkeyMVP);
    monkey.draw();

    //Swap the SDL window
    SDL_GL_SwapWindow(window);
  }

  // Cleanup SDL
  Mix_FreeChunk(jumpSound);
  Mix_FreeChunk(strangeSound);
  Mix_CloseAudio();
  SDL_GL_DeleteContext(glContext);
  SDL_DestroyWindow(window);
  IMG_Quit();
  SDL_Quit();

  return 0;
}

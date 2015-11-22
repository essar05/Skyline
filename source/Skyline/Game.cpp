#include "Game.h"
#include <Tile.h>
#include <iostream>
#include <ResourceManager.h>
#include <TextureCache.h>
#include <GLTexture.h>

Game::Game() {}

Game::~Game() {}

Game* Game::instance = NULL;

void Game::Boot() {
  initSystem();

  _state = GameState::RUNNING;
}

void Game::Run() {
  SDL_StartTextInput();

  while(_state == GameState::RUNNING) {
    float startTicks = SDL_GetTicks();

    processInput();
    render();

    float frameTicks = SDL_GetTicks() - startTicks;

    //limit FPS
    if(_maxFps > 0 && 1000.0f / _maxFps > frameTicks) {
      SDL_Delay(1000.0f / _maxFps - frameTicks);
    }

    //calculate FPS
    calculateFPS();

    if(_debugMode == true) {
      static int frameCounter = 0;
      frameCounter++;
      if(frameCounter == 20) {
        std::cout << this->_fps << std::endl;
        frameCounter = 0;
      }
    }
  }

  SDL_StopTextInput();
}

void Game::render() {
  glClearDepth(1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  _baseProgram->use();
  _camera.update();

  glActiveTexture(GL_TEXTURE0);
  GLint textureLocation = _baseProgram->getUniformLocation("textureSampler");
  glUniform1i(textureLocation, 0);

  GLint pLocation = _baseProgram->getUniformLocation("P");
  glm::mat4 cameraMatrix = _camera.getCameraMatrix();

  glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

  _spriteBatch.begin();

  glm::vec4 position(0.0f, 0.0f, 50.0f, 50.0f);
  glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
  Essengine::Color color;
  color.r = 255;
  color.g = 255;
  color.b = 255;
  color.a = 255;

  Essengine::TextureCache cache;

  Essengine::GLTexture tex = cache.getTexture("Textures/grass_tile.png");

  //_spriteBatch.draw(position, uv, Essengine::ResourceManager::getTexture("Textures/grass_tile.png")._id, color, 0);
  _spriteBatch.draw(position, uv, tex._id, color, 0);

  _spriteBatch.end();

  _spriteBatch.render();

  _baseProgram->unuse();

  _window->SwapBuffer();
}

void Game::processInput() {
  SDL_Event event;

  while(SDL_PollEvent(&event) != 0) {
    if(event.type == SDL_QUIT) {
      _state = GameState::EXIT;
    }
  }
}

void Game::initSystem() {
  SDL_Init(SDL_INIT_EVERYTHING);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  _window = new Essengine::Window("Skyline 0.1", 1024, 768, 0);

  initShaders();

  _camera.init(1024, 768);

  _spriteBatch.init();
}

void Game::initShaders() {
  _baseProgram = new Essengine::GLProgram(true);
  _baseProgram->loadShader(Essengine::ShaderType::VERTEX, "Shaders/Vertex.shader");
  _baseProgram->loadShader(Essengine::ShaderType::FRAGMENT, "Shaders/Fragment.shader");
  _baseProgram->compileShaders();

  _baseProgram->addAttribute("vertexPosition");
  _baseProgram->addAttribute("vertexColor");
  _baseProgram->addAttribute("vertexUV");

  _baseProgram->linkShaders();
}

void Game::calculateFPS() {
  static const int NUM_SAMPLES = 10;
  static float frameTimes[NUM_SAMPLES];

  static float prevTicks = SDL_GetTicks();
  static int currentFrame = 0;

  float currentTicks = SDL_GetTicks();

  _frameTime = currentTicks - prevTicks;
  //if it runs too fast, it puts the frame time as 0 so we just wanna approximate to 1
  if(_frameTime == 0) {
    _frameTime = 1;
  }

  frameTimes[currentFrame % NUM_SAMPLES] = _frameTime;
  
  prevTicks = currentTicks;
  currentFrame++;

  int sampleCount = NUM_SAMPLES;
  if(currentFrame < NUM_SAMPLES) {
    sampleCount = currentFrame;
  }

  float frameTimeAverage = 0;
  for(int i = 0; i < sampleCount; i++) {
    frameTimeAverage += frameTimes[i];
  }

  frameTimeAverage /= sampleCount;

  if(frameTimeAverage > 0) {
    _fps = 1000.0f / frameTimeAverage;
  } else {
    _fps = 7.0f;
  }
  
}

void Game::Destroy() {
  delete _window;
  delete _baseProgram;
  delete this;
}

Game* Game::GetInstance() {
  if (instance == NULL) {
    instance = new Game();
  }
  return instance;
}
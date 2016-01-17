#include "Game.h"
#include <iostream>
#include <ResourceManager.h>
#include <TextureCache.h>
#include <GLTexture.h>
#include <windows.h>
#include <Psapi.h>


Game::Game() {}

Game::~Game() {}

Game* Game::instance = NULL;

void Game::Boot() {
  initSystem();

  _state = GameState::RUNNING;

  float sectionWidth = 1024.0f;
  float sectionHeight = (float) (1024.0f * _textureCache.getTexture("Textures/tex1.png")._height) / _textureCache.getTexture("Textures/tex1.png")._width;

  LevelSection* section = new LevelSection(sectionWidth, sectionHeight);
  section->setBackground(_textureCache.getTexture("Textures/tex1.png")._id);
  _level.addSection(section);
  LevelSection* section2 = new LevelSection(sectionWidth, sectionHeight);
  section->setBackground(_textureCache.getTexture("Textures/tex1.png")._id);
  _level.addSection(section);

  _player = new Player(100, _textureCache.getTexture("Textures/tesx.png")._id, 100, 200, glm::vec2(500.0f, 100.0f));
  _player->setSpeed(8.0f);

  /*
    objects are only allowed to be in their respective sections
    that way, only the objects currently in view will be rendered.
    tentitymanager to add/get objects
    only spawn objects when they are in the next section, and they are about to enter the screen.
  */
}

void Game::Run() {
  SDL_StartTextInput();

  while(_state == GameState::RUNNING) {
    _fpsLimiter.begin();

    processInput();
    render();

    _fps = _fpsLimiter.end();

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
  /*
  PROCESS_MEMORY_COUNTERS memCounter;
  bool result = GetProcessMemoryInfo(GetCurrentProcess(), &memCounter, sizeof(memCounter));
  std::cout << "Start " << memCounter.WorkingSetSize / (1014) << " kb" << std::endl;
  */
  glActiveTexture(GL_TEXTURE0);
  GLint textureLocation = _baseProgram->getUniformLocation("textureSampler");
  glUniform1i(textureLocation, 0);

  GLint pLocation = _baseProgram->getUniformLocation("P");
  glm::mat4 cameraMatrix = _camera.getCameraMatrix();

  glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

  _spriteBatch.begin();
  /*
  glm::vec4 position(80.0f, 23.0f, 100.0f, 100.0f);
  glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
  Essengine::ColorRGBA8 color(255, 255, 255, 255);
  
  Essengine::GLTexture tex = _textureCache.getTexture("Textures/grass_tile.png");

  //result = GetProcessMemoryInfo(GetCurrentProcess(), &memCounter, sizeof(memCounter));
  //std::cout << "End " << memCounter.WorkingSetSize / (1014) << " kb" << std::endl << std::endl;

  _spriteBatch.draw(position, uv, _textureCache.getTexture("Textures/grass_tile.png")._id, color, 0);
  //_spriteBatch.draw(position, uv, tex._id, color, 0);
  */

  _level.draw();
  _player->draw();

  _spriteBatch.end();
  _spriteBatch.render();

  _camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, 2.0f));
  _player->move(8, 2.0f);

  _baseProgram->unuse();

  _window->SwapBuffer();
}

void Game::processInput() {
  SDL_Event event;

  const float CAMERA_SPEED = 20.0f;
  const float SCALE_SPEED = 0.01f;

  while(SDL_PollEvent(&event) != 0) {
    switch(event.type) {
      case SDL_QUIT:
        _state = GameState::EXIT;
        break;
      case SDL_KEYDOWN:
        _inputManager.pressKey(event.key.keysym.sym);
        break;
      case SDL_KEYUP:
        _inputManager.releaseKey(event.key.keysym.sym);
    }
  }

  if(_inputManager.isKeyDown(SDLK_w)) {
    _camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, CAMERA_SPEED));
  }
  if(_inputManager.isKeyDown(SDLK_s)) {
    _camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, -CAMERA_SPEED));
  }
  if(_inputManager.isKeyDown(SDLK_a)) {
    _camera.setPosition(_camera.getPosition() + glm::vec2(CAMERA_SPEED, 0.0f));
  }
  if(_inputManager.isKeyDown(SDLK_d)) {
    _camera.setPosition(_camera.getPosition() + glm::vec2(-CAMERA_SPEED, 0.0f));
  }
  if(_inputManager.isKeyDown(SDLK_q)) {
    _camera.setScale(_camera.getScale() + SCALE_SPEED);
  }
  if(_inputManager.isKeyDown(SDLK_e)) {
    _camera.setScale(_camera.getScale() - SCALE_SPEED);
  }
  if(_inputManager.isKeyDown(SDLK_LEFT)) {
    _player->move(4);
  }
  if(_inputManager.isKeyDown(SDLK_RIGHT)) {
    _player->move(6);
  }
  if(_inputManager.isKeyDown(SDLK_UP)) {
    _player->move(8);
  }
  if(_inputManager.isKeyDown(SDLK_DOWN)) {
    _player->move(2);
  }

  //std::cout << _camera.getPosition().x << ", " << _camera.getPosition().y << std::endl;
}

void Game::initSystem() {
  SDL_Init(SDL_INIT_EVERYTHING);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  _window = new Essengine::Window(this->_title, this->_width, this->_height, 0);

  initShaders();

  _camera.init(1024, 768);
  _spriteBatch.init();
  _fpsLimiter.init(_maxFPS);
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

Essengine::SpriteBatch* Game::getSpriteBatch() {
  return &_spriteBatch;
}

Essengine::Camera2D* Game::getMainCamera() {
  return &_camera;
}

float Game::getWidth() {
  return _width;
}

float Game::getHeight() {
  return _height;
}

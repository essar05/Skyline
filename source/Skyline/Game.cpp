#define NOMINMAX

#include "Game.h"
#include <iostream>
#include <ResourceManager.h>
#include <TextureCache.h>
#include <GLTexture.h>
#include <windows.h>
#include <algorithm>
//#include <Psapi.h>
#include "Entity.h"
#include "Utils.h"

Game::Game() {}

Game::~Game() {}

Game* Game::instance = NULL;

void Game::Boot() {
  initSystem();

  _state = GameState::RUNNING;

  _entityManager = new EntityManager;

  _level = new Level();
  _level->load("intro");
}

void Game::Run() {
  SDL_StartTextInput();

  const float DESIRED_FPS = 60;
  const float MS_PER_SECOND = 1000;
  const float DESIRED_FRAMETIME = MS_PER_SECOND / DESIRED_FPS;
  const int MAX_FRAMES_SIMULATED = 6;
  const float MAX_DELTA_TIME = 1.0f;

  float previousTicks = (float) SDL_GetTicks();

  while(_state == GameState::RUNNING) {
    _fpsLimiter.begin();

    float newTicks = (float) SDL_GetTicks();
    float frameTime = newTicks - previousTicks;
    previousTicks = newTicks;
    float totalDeltaTime = frameTime / DESIRED_FRAMETIME;
    
    int i = 0;
    while(totalDeltaTime > 0.0f && i < MAX_FRAMES_SIMULATED) {
      float deltaTime = std::min(totalDeltaTime, MAX_DELTA_TIME);

      processInput(deltaTime);
      update(deltaTime);

      totalDeltaTime -= deltaTime;
      i++;
    }

    Render();

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

void Game::update(float deltaTime) {
  if(!_isPaused) {
    _camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, this->scrollSpeed * _camera.getZoom()) * deltaTime);
    _level->update(deltaTime);
    updateProjectiles(deltaTime);
    updateObjects(deltaTime);
  }
}

void Game::updateObjects(float deltaTime) {
  /*Entity* entity;
  std::vector<unsigned int> activeObjects = _level->getActiveObjects();
  for(unsigned int i = 0; i < activeObjects.size(); i++) {
    entity = _entityManager.getEntity(activeObjects[i]);
    if(entity == nullptr) {
      continue;
    }

    //_player->collidesWith(entity);
  }*/
}

void Game::updateProjectiles(float deltaTime) {
  _projectileManager.update(deltaTime);
}

void Game::Render() {
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

  GLint useTextureLocation = _baseProgram->getUniformLocation("useTexture");
  glUniform1i(useTextureLocation, 1);

  GLint pLocation = _baseProgram->getUniformLocation("P");
  glm::mat4 cameraMatrix = _camera.getCameraMatrix();

  glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

  _spriteBatch.begin(Essengine::GlyphSortType::BACK_TO_FRONT);
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

  _level->draw();
  _projectileManager.draw();

  _spriteBatch.end();
  _spriteBatch.render();

  //unbind texture (not neccesary, or well doesn't do anything really)
  //glBindTexture(GL_TEXTURE_2D, 0);
  glUniform1i(useTextureLocation, 0);
  this->getLevel()->getWorld()->DrawDebugData();

  /*
  just as reminder of render process i guess?
  GLfloat vertices[] = {
    10.0f, 10.0f,  1.0f, 0.0f, 0.0f, 1.0f,
    10.0f, 5.0f,   1.0f, 0.0f, 0.0f, 1.0f,
    15.0f, 5.0f,   1.0f, 0.0f, 0.0f, 1.0f,
    15.0f, 5.0f,  1.0f, 0.0f, 0.0f, 1.0f,
    10.0f, 5.0f,   1.0f, 0.0f, 0.0f, 1.0f,
    10.0f, 10.0f,   1.0f, 0.0f, 0.0f, 1.0f
  };

  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  GLuint vbo;
  glGenBuffers(1, &vbo);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*) 0);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*) (2 * sizeof(GLfloat)));

  glDrawArrays(GL_TRIANGLES, 0, 3);

  glBindVertexArray(0);
  */

  _baseProgram->unuse();

  _window->SwapBuffer();
}

void Game::processInput(float deltaTime) {
  SDL_Event event;

  const float CAMERA_SPEED = 0.5f;
  const float SCALE_SPEED = 1.0f;

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
    _camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, CAMERA_SPEED * deltaTime));
  }
  if(_inputManager.isKeyDown(SDLK_s)) {
    _camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, -CAMERA_SPEED * deltaTime));
  }
  if(_inputManager.isKeyDown(SDLK_d)) {
    _camera.setPosition(_camera.getPosition() + glm::vec2(CAMERA_SPEED * deltaTime, 0.0f));
  }
  if(_inputManager.isKeyDown(SDLK_a)) {
    _camera.setPosition(_camera.getPosition() + glm::vec2(-CAMERA_SPEED * deltaTime, 0.0f));
  }
  if(_inputManager.isKeyDown(SDLK_q)) {
    _camera.setScale(_camera.getScale() + SCALE_SPEED * deltaTime);
  }
  if(_inputManager.isKeyDown(SDLK_e)) {
    _camera.setScale(_camera.getScale() - SCALE_SPEED * deltaTime);
  }
  if(_inputManager.isKeyDown(SDLK_TAB)) {
    if(_canPause == true) {
      _isPaused = !_isPaused;
      _canPause = false;
    }
  } else {
    _canPause = true;
  }

  glm::vec2 direction = glm::vec2(0.0f, 0.0f);

  if(_inputManager.isKeyDown(SDLK_LEFT)) {
    direction += glm::vec2(-1.0f, 0.0f);
  }
  if(_inputManager.isKeyDown(SDLK_RIGHT)) {
    direction += glm::vec2(1.0f, 0.0f);
  }
  if(_inputManager.isKeyDown(SDLK_UP)) {
    direction += glm::vec2(0.0f, 1.0f);
  }
  if(_inputManager.isKeyDown(SDLK_DOWN)) {
    direction += glm::vec2(0.0f, -1.0f);
  }

  _level->getPlayer()->setDirection(direction);
 
  if(_inputManager.isKeyDown(SDLK_SPACE)) {
    _level->getPlayer()->setIsFiring(true);
  } else {
    _level->getPlayer()->setIsFiring(false);
  }
}

void Game::initSystem() {
  SDL_Init(SDL_INIT_EVERYTHING);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  _window = new Essengine::Window(this->_title, (int) this->_width, (int) this->_height, 0);

  initShaders();

  _camera.init((int) this->_width, (int) this->_height);
  _camera.setScale(32.0f);
  _camera.setZoom(this->getWidth() / 1024.0f);
  _camera.setPosition(_camera.getWorldCoordinates(glm::vec2(this->_width / 2, this->_height / 2)));

  _spriteBatch.init();
  _fpsLimiter.init(_maxFPS, _limitFPS);
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
  delete _entityManager;
  delete _level;

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

EntityManager* Game::getEntityManager() {
  return _entityManager;
}

Essengine::TextureCache* Game::getTextureCache() {
  return &_textureCache;
}
#define NOMINMAX

#include "Game.h"
#include <iostream>
#include <ResourceManager.h>
#include <TextureCache.h>
#include <Texture2D.h>
#include <windows.h>
#include <algorithm>
//#include <Psapi.h>
#include "Entity.h"
#include "Utils.h"
#include <chrono>

Game::Game() {}

Game::~Game() {}

Game* Game::instance = NULL;

void Game::Boot() {
  std::cout << "Booting from " << SDL_GetBasePath() << std::endl;

  initSystem();

  _gui.init("GUI");
  _gui.loadScheme("TaharezLook.scheme");
  _gui.loadScheme("AlfiskoSkin.scheme");
  _gui.setFont("DejaVuSans-10");
  CEGUI::PushButton* button = static_cast<CEGUI::PushButton*> (
      _gui.createWidget("AlfiskoSkin/Button", glm::vec4(0.5f, 0.5f, 0.1f, 0.05f), glm::vec4(0.0f), "Button")
    );
  button->setText("Test GUI");

  _gui.setMouseCursor("TaharezLook/MouseArrow");
  _gui.showMouseCursor();

  CEGUI::Editbox* editbox = static_cast<CEGUI::Editbox*> (
    _gui.createWidget("AlfiskoSkin/Editbox", glm::vec4(0.2f, 0.2f, 0.1f, 0.05f), glm::vec4(0.0f), "Editbox")
    );

  _state = GameState::RUNNING;

  _entityManager = new EntityManager;
  _projectileManager = new ProjectileManager;

  _level = new Level();
  _level->load("intro");

  _audioManager.loadBank("Sounds/Master Bank.bank", FMOD_STUDIO_LOAD_BANK_NORMAL);
  _audioManager.loadBank("Sounds/Master Bank.strings.bank", FMOD_STUDIO_LOAD_BANK_NORMAL);
  _audioManager.loadBank("Sounds/Music.bank", FMOD_STUDIO_LOAD_BANK_NORMAL);
  _audioManager.loadBank("Sounds/Effects.bank", FMOD_STUDIO_LOAD_BANK_NORMAL);
}

void Game::Run() {
  SDL_StartTextInput();

  const int MAX_FRAMES_SIMULATED = 6;
  const float TIMESTEP = 1.0f / 60.0f; //each physics step should represent a 60th of a second advancement in the world
  static float deltaBuffer = 0; // this buffer keeps track of the extra bits of time

  std::chrono::high_resolution_clock::time_point previousTicks = std::chrono::high_resolution_clock::now(); // microseconds
  std::chrono::high_resolution_clock::time_point newTicks = previousTicks; // microseconds
  float frametime = 0.0f;

  //temporarily disabled, cause it's very annoying when debugging (perhaps when playing too, lol)
  _audioManager.playEvent("event:/music/heavyrain_david");
  _audioManager.playEvent("event:/effects/ambience_ship");

  while(_state == GameState::RUNNING) {
    _fpsLimiter.begin();

    newTicks = std::chrono::high_resolution_clock::now(); // microseconds
    frametime = std::chrono::duration_cast<std::chrono::microseconds>(newTicks - previousTicks).count() / 1000.0f; //in miliseconds
    previousTicks = newTicks;

    _timestepAccumulator += frametime / 1000.0f;
    const int nSteps = static_cast<int> ( std::floor(_timestepAccumulator / TIMESTEP) );
    if(nSteps > 0) {
      _timestepAccumulator -= nSteps * TIMESTEP;
    }
    _timestepAccumulatorRatio = _timestepAccumulator / TIMESTEP;

    const int nStepsClamped = std::min(nSteps, MAX_FRAMES_SIMULATED);
    //this->processInput(TIMESTEP * nStepsClamped);
    for(int i = 0; i < nStepsClamped; i++) {
      this->processInput(TIMESTEP);
      _level->resetSmoothStates();
      this->update(TIMESTEP);
    }
    
    //smooth cameraPosition as well. maybe we could do it inside smoothStates so we don't have separated code but for now this will do
    if (this->_isPaused == true) {
      _camera.setPosition(_cameraPosition);
    } else {
      const float oneMinusRatio = 1.f - _timestepAccumulatorRatio;
      glm::vec2 interpolatedCameraPosition = _timestepAccumulatorRatio * _cameraPosition + oneMinusRatio * _previousCameraPosition;
      _camera.setPosition(interpolatedCameraPosition);
    }
    
    _level->smoothStates();
    _level->getWorld()->ClearForces();

    _camera.update();

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
    _previousCameraPosition = _cameraPosition;
    _cameraPosition = _cameraPosition + glm::vec2(0.0f, this->_scrollSpeed * _camera.getZoom()) * deltaTime;
    
    _projectileManager->update(deltaTime);
    _level->update(deltaTime);
    _entityManager->deleteQueuedEntities();
    _projectileManager->deleteQueuedProjectiles();
  }
  _audioManager.update();
}

void Game::Render() {
  glClearDepth(1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  //bind FBO, all rendering will be done to this FBO's color buffer
  _sceneFBO->bind();
  //render scene
  _sceneRenderer->render();
  //unbind FBO, rendering will now be done to screen
  _sceneFBO->unbind();
  
  //apply post processing effects
  _postProcessing->applyEffects(_sceneFBO);

  //render the FBO color texture.
  _fboRenderer->render(_postProcessing->getResult());

  //draw GUI
  _gui.draw();

  //swap front and back buffer to display stuff to screen
  _window->SwapBuffer();
}

void Game::processInput(float deltaTime) {
  SDL_Event event;

  const float CAMERA_SPEED = 0.5f;
  const float SCALE_SPEED = 5.0f;

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

    _gui.onSDLEvent(event);
  }

  if (_inputManager.isKeyDown(SDLK_ESCAPE)) {
    _state = GameState::EXIT;
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

  _window = new Ess2D::Window(this->_title, (int) this->_width, (int) this->_height, this->_windowMode);
  _window->SetVSync(_vSync);

  _sceneRenderer = new SceneRenderer();
  _fboRenderer = new FBORenderer();
  _fboRenderer->initShader();
  _sceneFBO = new Ess2D::FrameBufferObject(_window, (GLsizei) _width, (GLsizei) _height, Ess2D::DepthBufferType::NONE);
  _postProcessing = new PostProcessing();

  _camera.init((int) this->_width, (int) this->_height);
  _camera.setScale(32.0f);
  _camera.setZoom(this->getWidth() / 1024.0f);
  _camera.setPosition(_camera.getWorldCoordinates(glm::vec2(this->_width / 2, this->_height / 2)));
  _previousCameraPosition = _camera.getPosition();
  _cameraPosition = _previousCameraPosition;

  _spriteBatch.init();
  _fpsLimiter.init(_maxFPS, _limitFPS);
}

void Game::Destroy() {
  delete _sceneRenderer;
  delete _fboRenderer;
  delete _postProcessing;
  delete _sceneFBO;
  delete _window;
  delete _entityManager;
  _entityManager = nullptr;
  delete _projectileManager;
  _projectileManager = nullptr;
  delete _level;
  _level = nullptr;
  
  delete this;
}

Game* Game::GetInstance() {
  if (instance == NULL) {
    instance = new Game();
  }
  return instance;
}

Ess2D::SpriteBatch* Game::getSpriteBatch() {
  return &_spriteBatch;
}

Ess2D::Camera2D* Game::getMainCamera() {
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

Ess2D::TextureCache* Game::getTextureCache() {
  return &_textureCache;
}

Ess2D::AudioManager* Game::getAudioManager() {
  return &_audioManager;
}

SceneRenderer* Game::getSceneRenderer() {
  return _sceneRenderer;
}

Ess2D::Window* Game::getWindow() {
  return _window;
}
#include "GameplayScreen.h"
#include <SDL_events.h>

GameplayScreen::GameplayScreen() {
  _game = Game::GetInstance();
}

GameplayScreen::~GameplayScreen() {}

int GameplayScreen::getNextScreenIndex() const {
  return SCREEN_INDEX_NO_SCREEN;
}

int GameplayScreen::getPreviousScreenIndex() const {
  return SCREEN_INDEX_NO_SCREEN;
}


void GameplayScreen::onEntry() {

}

void GameplayScreen::onExit() {

}

void GameplayScreen::build() {
  _entityManager = new EntityManager;
  _projectileManager = new ProjectileManager;

  _camera.init((int)_game->getWidth(), (int)_game->getHeight());
  _camera.setScale(32.0f);
  _camera.setZoom(_game->getWidth() / 1024.0f);
  glm::vec2 initialCameraPosition = _camera.getWorldCoordinates(glm::vec2(_game->getWidth() / 2, _game->getHeight() / 2));
  _camera.setPosition(initialCameraPosition);
  _camera.setFuturePosition(initialCameraPosition);

  _spriteBatch.init();

  _sceneRenderer = new SceneRenderer();
  _fboRenderer = new FBORenderer();
  _fboRenderer->initShader();
  _sceneFBO = new Ess2D::FrameBufferObject(_game->getWindow(), (GLsizei) _game->getWidth(), (GLsizei) _game->getHeight(), Ess2D::DepthBufferType::NONE);
  _postProcessing = new PostProcessing();

  _level = new Level();
  _level->load("intro");
}

void GameplayScreen::destroy() {
  delete _sceneRenderer;
  delete _fboRenderer;
  delete _postProcessing;
  delete _sceneFBO;
  delete _entityManager;
  _entityManager = nullptr;
  delete _projectileManager;
  _projectileManager = nullptr;
  delete _level;
  _level = nullptr;
}

void GameplayScreen::draw() {
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
}

void GameplayScreen::update(float deltaTime, int simulationSteps) {
  for(int i = 0; i < simulationSteps; i++) {
    processInput(deltaTime);
    _level->resetSmoothStates();

    if(!_isPaused) {
      _camera.setFuturePosition(_camera.getFuturePosition() + glm::vec2(0.0f, this->_scrollSpeed * _camera.getZoom()) * deltaTime);

      _projectileManager->update(deltaTime);
      _level->update(deltaTime);
      _entityManager->deleteQueuedEntities();
      _projectileManager->deleteQueuedProjectiles();
    }
  }
  
  _camera.smoothState(_game->getTimestepAccumulator()->getAccumulatorRatio(), _isPaused);
  _level->smoothStates();
  _level->getWorld()->ClearForces();

  _camera.update();
}

void GameplayScreen::processInput(float deltaTime) {
  SDL_Event event;

  //MOVE THESE FROM HERE
  const float CAMERA_SPEED = 0.5f;
  const float SCALE_SPEED = 5.0f;

  while(SDL_PollEvent(&event) != 0) {
    _game->onSDLEvent(event);

    //_gui.onSDLEvent(event);
  }

  Ess2D::InputManager* inputManager = _game->getInputManager();

  if(inputManager->isKeyDown(SDLK_ESCAPE)) {
    _currentState = Ess2D::ScreenState::EXIT_APPLICATION;
  }

  if(inputManager->isKeyDown(SDLK_w)) {
    _camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, CAMERA_SPEED * deltaTime));
  }
  if(inputManager->isKeyDown(SDLK_s)) {
    _camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, -CAMERA_SPEED * deltaTime));
  }
  if(inputManager->isKeyDown(SDLK_d)) {
    _camera.setPosition(_camera.getPosition() + glm::vec2(CAMERA_SPEED * deltaTime, 0.0f));
  }
  if(inputManager->isKeyDown(SDLK_a)) {
    _camera.setPosition(_camera.getPosition() + glm::vec2(-CAMERA_SPEED * deltaTime, 0.0f));
  }
  if(inputManager->isKeyDown(SDLK_q)) {
    _camera.setScale(_camera.getScale() + SCALE_SPEED * deltaTime);
  }
  if(inputManager->isKeyDown(SDLK_e)) {
    _camera.setScale(_camera.getScale() - SCALE_SPEED * deltaTime);
  }
  if(inputManager->isKeyDown(SDLK_TAB)) {
    if(_canPause == true) {
      _isPaused = !_isPaused;
      _canPause = false;
    }
  } else {
    _canPause = true;
  }

  glm::vec2 direction = glm::vec2(0.0f, 0.0f);

  if(inputManager->isKeyDown(SDLK_LEFT)) {
    direction += glm::vec2(-1.0f, 0.0f);
  }
  if(inputManager->isKeyDown(SDLK_RIGHT)) {
    direction += glm::vec2(1.0f, 0.0f);
  }
  if(inputManager->isKeyDown(SDLK_UP)) {
    direction += glm::vec2(0.0f, 1.0f);
  }
  if(inputManager->isKeyDown(SDLK_DOWN)) {
    direction += glm::vec2(0.0f, -1.0f);
  }

  _level->getPlayer()->setDirection(direction);

  if(inputManager->isKeyDown(SDLK_SPACE)) {
    _level->getPlayer()->setIsFiring(true);
  } else {
    _level->getPlayer()->setIsFiring(false);
  }

}

Ess2D::SpriteBatch* GameplayScreen::getSpriteBatch() { return &_spriteBatch; }
Ess2D::Camera2D* GameplayScreen::getMainCamera() { return &_camera; }
Ess2D::TextureCache* GameplayScreen::getTextureCache() { return &_textureCache; }
EntityManager* GameplayScreen::getEntityManager() { return _entityManager; }
Level* GameplayScreen::getLevel() { return _level; }
ProjectileManager* GameplayScreen::getProjectileManager() { return _projectileManager; }
SceneRenderer* GameplayScreen::getSceneRenderer() { return _sceneRenderer; }
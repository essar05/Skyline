#include "GameplayScreen.h"
#include <SDL_events.h>
#include "SpaceshipA.h"

GameplayScreen::GameplayScreen() {
  _game = Game::GetInstance();
  _screenIndex = SCREEN_INDEX_GAMEPLAY;
}

GameplayScreen::~GameplayScreen() {}

int GameplayScreen::getNextScreenIndex() const {
  return SCREEN_INDEX_NO_SCREEN;
}

int GameplayScreen::getPreviousScreenIndex() const {
  return SCREEN_INDEX_MAINMENU;
}

void GameplayScreen::onEntry() {
  _game->getAudioManager()->playEvent("event:/effects/ambience_ship");

  _gui.getRootWindow()->getChild("HUDRoot/Default")->setVisible(true);
  _gui.getRootWindow()->getChild("HUDRoot/GameOverRoot")->setVisible(false);
  _gui.getRootWindow()->getChild("HUDRoot/VictoryRoot")->setVisible(false);
  _gui.getRootWindow()->getChild("HUDRoot/PauseRoot")->setVisible(false);

  restartLevel();
}

void GameplayScreen::onExit() {
  _game->getAudioManager()->stopEvent("event:/effects/ambience_ship", true);
}

void GameplayScreen::build() {
  _entityManager = new EntityManager;
  _projectileManager = new ProjectileManager;

  _camera.init((int)_game->getWidth(), (int)_game->getHeight());
  _camera.setScale(32.0f);
  _camera.setZoom(_game->getWidth() / _targetWidth);
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

  _level->getPlayer()->setDefaultVelocity(glm::vec2(0.0f, _scrollSpeed));

  //preload explosion 
  _textureCache.getAtlas("Textures/Explosions/explosion1.png", "Textures/Explosions/explosion1.json");
  _textureCache.getAtlas("Textures/Explosions/explosion2.png", "Textures/Explosions/explosion2.json");
  _textureCache.getAtlas("Textures/Explosions/explosion3.png", "Textures/Explosions/explosion3.json");
  _textureCache.getAtlas("Textures/Explosions/explosion4.png", "Textures/Explosions/explosion4.json");
  _textureCache.getAtlas("Textures/Explosions/explosion5.png", "Textures/Explosions/explosion5.json");

  initUI();
}

void GameplayScreen::destroy() {
  delete _sceneRenderer;
  delete _fboRenderer;
  delete _postProcessing;
  delete _sceneFBO;
  delete _projectileManager;
  _projectileManager = nullptr;
  //delete _entityManager;
  //_entityManager = nullptr;
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

  _gui.draw();
}

void GameplayScreen::update(float deltaTime, int simulationSteps) {
  if(_reloadLevel) {
    restartLevel(false);

    _reloadLevel = false;
  } else {
    for(int i = 0; i < simulationSteps; i++) {
      processInput(deltaTime);
      _level->resetSmoothStates();

      if(!_isPaused) {
        if(!_isFreezed) {
          if(_level->getPlayer()->getDefaultVelocity().y == 0.0f) {
            _level->getPlayer()->setDefaultVelocity(glm::vec2(0.0f, _scrollSpeed));
          }
          _camera.setFuturePosition(_camera.getFuturePosition() + glm::vec2(0.0f, this->_scrollSpeed * _camera.getZoom()) * deltaTime);
        } else {
          if(_level->getPlayer()->getDefaultVelocity().y > 0.0f) {
            _level->getPlayer()->setDefaultVelocity(glm::vec2(0.0f, 0.0f));
          }
        }

        _projectileManager->update(deltaTime);
        _level->update(deltaTime);
        _entityManager->deleteQueuedEntities();
        _projectileManager->deleteQueuedProjectiles();
        _particleManager.update(deltaTime);

        _game->getGameplayScreen()->addGameTime(deltaTime);

        checkVictory();
      }
    }

    _camera.smoothState(_game->getTimestepAccumulator()->getAccumulatorRatio(), _isPaused || _isFreezed);
    _level->smoothStates();
    _level->getWorld()->ClearForces();
  }

  _camera.update();

  _gui.getRootWindow()->getChild("HUDRoot/Default/Life/HP")->setText(std::to_string((int) _level->getPlayer()->getHealth()) + "%");
  _gui.getRootWindow()->getChild("HUDRoot/Default/Life/Score")->setText(std::to_string(_score));
}

void GameplayScreen::checkVictory() {
  if(_level->isAtEnd()) {
    displayVictoryScreen();
  }

  if(_level->getPlayer()->isDead()) {
    _lives--;
    
    if(_lives == 2) {
      _gui.getRootWindow()->getChild("HUDRoot/Default/HeartIcon3")->setVisible(false);
    }

    if(_lives == 1) {
      _gui.getRootWindow()->getChild("HUDRoot/Default/HeartIcon2")->setVisible(false);
    }

    if(_lives == 0) {
      _gui.getRootWindow()->getChild("HUDRoot/Default/HeartIcon1")->setVisible(false);
      displayGameOverScreen();
    } else {
      _level->getPlayer()->setHealth(100);
    }
  }
}

void GameplayScreen::processInput(float deltaTime) {
  SDL_Event event;

  //MOVE THESE FROM HERE
  const float CAMERA_SPEED = 30.0f;
  const float SCALE_SPEED = 5.0f;

  while(SDL_PollEvent(&event) != 0) {
    _game->onSDLEvent(event);
    _gui.onSDLEvent(event);
  }

  Ess2D::InputManager* inputManager = _game->getInputManager();

  if(inputManager->isKeyDown(SDLK_ESCAPE)) {
    _currentState = Ess2D::ScreenState::EXIT_APPLICATION;
  }

  if(inputManager->isKeyDown(SDLK_w)) {
    _camera.setFuturePosition(_camera.getFuturePosition() + glm::vec2(0.0f, CAMERA_SPEED * deltaTime));
  }
  if(inputManager->isKeyDown(SDLK_s)) {
    _camera.setFuturePosition(_camera.getFuturePosition() + glm::vec2(0.0f, -CAMERA_SPEED * deltaTime));
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

bool GameplayScreen::onBtnExitGameClicked(const CEGUI::EventArgs &e) {
  _currentState = Ess2D::ScreenState::EXIT_APPLICATION;
  return true;
}

bool GameplayScreen::onBtnFreezeGameClicked(const CEGUI::EventArgs &e) {
  _isFreezed = !_isFreezed;
  return true;
}

bool GameplayScreen::onBtnReloadLevelClicked(const CEGUI::EventArgs &e) {
  _reloadLevel = true;

  return true;
}

bool GameplayScreen::onBtnPauseGameClicked(const CEGUI::EventArgs &e) {
  pause();

  return true;
}

bool GameplayScreen::onBtnResumeGameClicked(const CEGUI::EventArgs &e) {
  resume();

  return true;
}

bool GameplayScreen::onBtnNextLevelClicked(const CEGUI::EventArgs &e) {
  _currentState = Ess2D::ScreenState::CHANGE_PREV;

  return true; 
}

bool GameplayScreen::onBtnRestartLevelClicked(const CEGUI::EventArgs &e) {
  restartLevel(true);
  _gui.getRootWindow()->getChild("HUDRoot/Default")->setVisible(true);
  _gui.getRootWindow()->getChild("HUDRoot/GameOverRoot")->setVisible(false);

  return true;
}

void GameplayScreen::pause() {
  _isPaused = true;

  _gui.getRootWindow()->getChild("HUDRoot/Default")->setVisible(false);
  _gui.getRootWindow()->getChild("HUDRoot/PauseRoot")->setVisible(true);
}

void GameplayScreen::resume() {
  _isPaused = false;

  _gui.getRootWindow()->getChild("HUDRoot/Default")->setVisible(true);
  _gui.getRootWindow()->getChild("HUDRoot/PauseRoot")->setVisible(false);
}

void GameplayScreen::restartLevel(bool resetCamera) {
  _isPaused = false;

  _particleManager.cleanup();

  delete _level;

  if(resetCamera) {
    _camera.init((int)_game->getWidth(), (int)_game->getHeight());
    _camera.setScale(32.0f);
    _camera.setZoom(_game->getWidth() / _targetWidth);
    glm::vec2 initialCameraPosition = _camera.getWorldCoordinates(glm::vec2(_game->getWidth() / 2, _game->getHeight() / 2));
    _camera.setPosition(initialCameraPosition);
    _camera.setFuturePosition(initialCameraPosition);

    _score = 0;
    _shotsFired = 0;
    _shotsHit = 0;
    _enemiesShotDown = 0;
    _gameTime = 0;
    _lives = 3;

    _gui.getRootWindow()->getChild("HUDRoot/Default/HeartIcon1")->setVisible(true);
    _gui.getRootWindow()->getChild("HUDRoot/Default/HeartIcon2")->setVisible(true);
    _gui.getRootWindow()->getChild("HUDRoot/Default/HeartIcon3")->setVisible(true);
  }

  _level = new Level();
  _level->load(_levelName);
  _level->getPlayer()->setDefaultVelocity(glm::vec2(0.0f, _scrollSpeed));
}

void GameplayScreen::initUI() {
  _gui.init("GUI");
  _gui.loadScheme("TaharezLook.scheme");
  _gui.loadScheme("AlfiskoSkin.scheme");
  _gui.loadScheme("Generic.scheme");
  _gui.loadScheme("Skyline.scheme");
  _gui.setFont("DejaVuSans-10");

  _gui.setMouseCursor("TaharezLook/MouseArrow");
  _gui.showMouseCursor();
  
  CEGUI::Window* hudRoot = _gui.loadLayout("SkylineHUD.layout");
  hudRoot->getChild("Default/PauseButton")->subscribeEvent(CEGUI::DefaultWindow::EventMouseClick, CEGUI::Event::Subscriber(&GameplayScreen::onBtnPauseGameClicked, this));
  hudRoot->getChild("PauseRoot/ContinueBtnContainer")->subscribeEvent(CEGUI::DefaultWindow::EventMouseClick, CEGUI::Event::Subscriber(&GameplayScreen::onBtnResumeGameClicked, this));
  hudRoot->getChild("PauseRoot/QuitBtnContainer")->subscribeEvent(CEGUI::DefaultWindow::EventMouseClick, CEGUI::Event::Subscriber(&GameplayScreen::onBtnExitGameClicked, this));

  hudRoot->getChild("VictoryRoot/ContinueBtnContainer")->subscribeEvent(CEGUI::DefaultWindow::EventMouseClick, CEGUI::Event::Subscriber(&GameplayScreen::onBtnNextLevelClicked, this));
  hudRoot->getChild("VictoryRoot/QuitBtnContainer")->subscribeEvent(CEGUI::DefaultWindow::EventMouseClick, CEGUI::Event::Subscriber(&GameplayScreen::onBtnExitGameClicked, this));

  hudRoot->getChild("GameOverRoot/RestartBtnContainer")->subscribeEvent(CEGUI::DefaultWindow::EventMouseClick, CEGUI::Event::Subscriber(&GameplayScreen::onBtnRestartLevelClicked, this));
  hudRoot->getChild("GameOverRoot/QuitBtnContainer")->subscribeEvent(CEGUI::DefaultWindow::EventMouseClick, CEGUI::Event::Subscriber(&GameplayScreen::onBtnExitGameClicked, this));
  /*
  CEGUI::PushButton* btn_exitGame = static_cast<CEGUI::PushButton*> (
    _gui.createWidget("AlfiskoSkin/Button", glm::vec4(1.0f - 0.09f, 1.0f - 0.0f - 0.04f - 0.04f, 0.09f, 0.04f), glm::vec4(0.0f), "Btn_ExitGame")
  );
  btn_exitGame->setWantsMultiClickEvents(false);
  btn_exitGame->setText("Exit");
  btn_exitGame->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameplayScreen::onBtnExitGameClicked, this));

  CEGUI::PushButton* btn_freezeGame = static_cast<CEGUI::PushButton*> (
    _gui.createWidget("AlfiskoSkin/Button", glm::vec4(1.0f - 0.09f, 1.0f - 0.04f - 0.04f - 0.04f, 0.09f, 0.04f), glm::vec4(0.0f), "btn_freezeGame")
  );
  btn_freezeGame->setWantsMultiClickEvents(false);
  btn_freezeGame->setText("Freeze");
  btn_freezeGame->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameplayScreen::onBtnFreezeGameClicked, this));
  
  CEGUI::PushButton* btn_reloadLevel = static_cast<CEGUI::PushButton*> (
    _gui.createWidget("AlfiskoSkin/Button", glm::vec4(1.0f - 0.09f, 1.0f - 0.04f - 0.04f - 0.04f - 0.04f, 0.09f, 0.04f), glm::vec4(0.0f), "btn_reloadLevel")
    );
  btn_reloadLevel->setWantsMultiClickEvents(false);
  btn_reloadLevel->setText("Reload Level");
  btn_reloadLevel->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameplayScreen::onBtnReloadLevelClicked, this));
  */
}

bool GameplayScreen::isFreezed() {
  return _isFreezed;
}

bool GameplayScreen::isPaused() {
  return _isPaused;
}

void GameplayScreen::addScore(int points) {
  _score += points;
}

void GameplayScreen::addShotsFired(int shots) {
  _shotsFired += shots;
}

void GameplayScreen::addShotsHit(int shots) {
  _shotsHit += shots;
}

void GameplayScreen::addEnemyShot() {
  _enemiesShotDown++;
}

void GameplayScreen::addGameTime(float deltaTime) {
  _gameTime += deltaTime;
}


std::string GameplayScreen::getGameTime() {
  int minutesElapsed = std::floor(_gameTime / 60);
  int secondsElapsed = std::round(_gameTime - minutesElapsed);

  std::string gameTimeString = std::to_string(minutesElapsed) + ":" + std::to_string(secondsElapsed);
  return gameTimeString;
}

std::string GameplayScreen::getAccuracy() {
  float accuracy = _shotsFired > 0 ? (float)_shotsHit / _shotsFired * 100 : 0.0f;

  char accuracyStringBuffer[100];
  snprintf(accuracyStringBuffer, sizeof(accuracyStringBuffer), "%.2f%%", accuracy);
  std::string accuracyString = accuracyStringBuffer;
  return accuracyString;
}

void GameplayScreen::displayVictoryScreen() {
  _isPaused = true;
  _gui.getRootWindow()->getChild("HUDRoot/Default")->setVisible(false);

  _gui.getRootWindow()->getChild("HUDRoot/VictoryRoot/ScoreValue")->setText(std::to_string(_score));
  _gui.getRootWindow()->getChild("HUDRoot/VictoryRoot/AccuracyValue")->setText(this->getAccuracy());
  _gui.getRootWindow()->getChild("HUDRoot/VictoryRoot/EnemiesShotValue")->setText(std::to_string(_enemiesShotDown));
  _gui.getRootWindow()->getChild("HUDRoot/VictoryRoot/GameTimeValue")->setText(this->getGameTime());

  _gui.getRootWindow()->getChild("HUDRoot/VictoryRoot")->setVisible(true);
}

void GameplayScreen::displayGameOverScreen() {
  _isPaused = true;
  _gui.getRootWindow()->getChild("HUDRoot/Default")->setVisible(false);

  _gui.getRootWindow()->getChild("HUDRoot/GameOverRoot/ScoreValue")->setText(std::to_string(_score));
  _gui.getRootWindow()->getChild("HUDRoot/GameOverRoot/AccuracyValue")->setText(this->getAccuracy());
  _gui.getRootWindow()->getChild("HUDRoot/GameOverRoot/EnemiesShotValue")->setText(std::to_string(_enemiesShotDown));
  _gui.getRootWindow()->getChild("HUDRoot/GameOverRoot/GameTimeValue")->setText(this->getGameTime());

  _gui.getRootWindow()->getChild("HUDRoot/GameOverRoot")->setVisible(true);
}

Ess2D::SpriteBatch* GameplayScreen::getSpriteBatch() { return &_spriteBatch; }
Ess2D::Camera2D* GameplayScreen::getMainCamera() { return &_camera; }
Ess2D::TextureCache* GameplayScreen::getTextureCache() { return &_textureCache; }
EntityManager* GameplayScreen::getEntityManager() { return _entityManager; }
Level* GameplayScreen::getLevel() { return _level; }
ProjectileManager* GameplayScreen::getProjectileManager() { return _projectileManager; }
SceneRenderer* GameplayScreen::getSceneRenderer() { return _sceneRenderer; }
ParticleManager* GameplayScreen::getParticleManager() { return &_particleManager; }
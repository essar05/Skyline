#include "IGame.h"

namespace Ess2D {

  IGame::IGame() {
    _screenManager = new ScreenManager();
  }

  IGame::~IGame() {
    delete _window;
  }

  void IGame::boot() {
    std::cout << "Booting from " << SDL_GetBasePath() << std::endl;

    init();
    _state = GameState::RUNNING;
  }

  void IGame::destroy() {
    if(_state != GameState::DESTROYED) {
      _state = GameState::DESTROYED;

      SDL_StopTextInput();
      if(_screenManager) {
        delete _screenManager;
      }
      onExit();
    }
  }

  void IGame::run() {
    _fpsLimiter.init(_maxFPS, _limitFPS);
    _timestepAccumulator.init();
    const float timestep = _timestepAccumulator.getTimestep();

    _state = GameState::RUNNING;
    while(_state == GameState::RUNNING) {
      _fpsLimiter.begin();
      const int simulationSteps = _timestepAccumulator.step();

      update(timestep, simulationSteps);
      render();

      _fps = _fpsLimiter.end();

      _window->SwapBuffer();
    }
  }

  bool IGame::init() {
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_ShowCursor(false);
    SDL_StartTextInput();

    initSystems();

    onInit();
    addScreens();

    _currentScreen = _screenManager->getCurrent();
    _currentScreen->onEntry();
    _currentScreen->setRunning();

    return true;
  }

  bool IGame::initSystems() {
    _fpsLimiter.init(_maxFPS, _limitFPS);

    _window = new Window(this->_title, (int) this->_width, (int) this->_height, this->_windowMode);
    _window->SetVSync(_vSync);

    return true;
  }

  void IGame::onSDLEvent(SDL_Event& evnt) {
    switch(evnt.type) {
      case SDL_QUIT:
        _state = GameState::EXIT;
        break;
      case SDL_KEYDOWN:
        _inputManager.pressKey(evnt.key.keysym.sym);
        break;
      case SDL_KEYUP:
        _inputManager.releaseKey(evnt.key.keysym.sym);
    }
  }

  void IGame::update(float deltaTime, int simulationSteps) {
    if(_currentScreen) {
      switch(_currentScreen->getState()) {
        case ScreenState::RUNNING:
          _currentScreen->update(deltaTime, simulationSteps);
          break;
        case ScreenState::CHANGE_NEXT:
          _currentScreen->onExit();
          _currentScreen = _screenManager->moveNext();
          std::cout << "change next";
          if(_currentScreen != nullptr) {
            _currentScreen->setRunning();
            _currentScreen->onEntry();
          }
          break;
        case ScreenState::CHANGE_PREV:
          _currentScreen->onExit();
          _currentScreen = _screenManager->movePrevious();
          if(_currentScreen != nullptr) {
            _currentScreen->setRunning();
            _currentScreen->onEntry();
          }
          break;
        case ScreenState::EXIT_APPLICATION:
          destroy();
          break;
        default:
          break;
      }
    } else {
      destroy();
    }

    onUpdate();
  }

  void IGame::render() {
    if(_currentScreen && _currentScreen->getState() == ScreenState::RUNNING) {
      _currentScreen->draw();
    }
  }

  float IGame::getWidth() {
    return _width;
  }

  float IGame::getHeight() {
    return _height;
  }

  const float IGame::getFPS() const {
    return _fps;
  }

  Window* IGame::getWindow() {
    return _window;
  }

  TimestepAccumulator* IGame::getTimestepAccumulator() {
    return &_timestepAccumulator;
  }

  InputManager* IGame::getInputManager() {
    return &_inputManager;
  }

};
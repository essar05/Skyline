#pragma once
#include "ScreenManager.h"
#include "Window.h"
#include "Timing.h"
#include "InputManager.h"

namespace Ess2D {

  enum class GameState { RUNNING, EXIT, DESTROYED };

  class IGame
  {
    public:
      IGame();
      virtual ~IGame();

      virtual void boot();
      virtual void destroy();
      virtual void run();

      virtual void addScreens() = 0;
      virtual void onInit() = 0;
      virtual void onExit() = 0;
      void onSDLEvent(SDL_Event& event);

      float getWidth();
      float getHeight();
      const float getFPS() const;

      Window* getWindow();
      TimestepAccumulator* getTimestepAccumulator();
      InputManager* getInputManager();

    protected:
      void update(float deltaTime, int simulationSteps);
      void render();

      virtual void onUpdate() {}

      bool init();
      bool initSystems();
      
      float _fps = 0.0f;
      float _maxFPS = 60.0f;
      bool _debugMode = false;
      bool _limitFPS = false;

      std::string _title;
      WindowMode _windowMode;
      float _width;
      float _height;
      bool _vSync;

      ScreenManager* _screenManager;
      IGameScreen* _currentScreen = nullptr;
      GameState _state;
      FPSLimiter _fpsLimiter;
      TimestepAccumulator _timestepAccumulator;
      InputManager _inputManager;

      Window* _window;

  };

}
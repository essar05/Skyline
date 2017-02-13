#pragma once
#include <chrono>

namespace Ess2D {

  class FPSLimiter {
    public:
      FPSLimiter();
      void init(float maxFPS, bool limitFPS);

      void begin();
      float end(); //returns current FPS

      void setMaxFPS(float fps);
      float getFPS() { return _fps; }

    private:
      void calculateFPS();
    
      float _maxFPS;

      std::chrono::high_resolution_clock::time_point _startTicks;
      std::chrono::high_resolution_clock::time_point _newTicks;
      float _fps;
      float _frameTime;
      float _time;
      bool _limitFPS;

  };

}
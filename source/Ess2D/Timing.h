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

  class TimestepAccumulator {
    public:
      TimestepAccumulator();
      ~TimestepAccumulator();

      void init();
      int step();

      float getTimestep() { return _timestep; }
      float getAccumulator() { return _accumulator; }
      float getAccumulatorRatio() { return _accumulatorRatio; }

    private:
      int _maxFramesSimulated = 6;
      float _timestep = 1.0f / 60.f;
      float _frametime = 0.0f;
      float _accumulator = 0.0f;
      float _accumulatorRatio = 0.0f;
      std::chrono::high_resolution_clock::time_point _prevTicks;
      std::chrono::high_resolution_clock::time_point _newTicks;

  };

}
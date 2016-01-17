#pragma once

namespace Essengine {

  class FPSLimiter {
    public:
      FPSLimiter();
      void init(float maxFPS);

      void begin();
      float end(); //returns current FPS

      void setMaxFPS(float fps);

    private:
      void calculateFPS();
    
      float _maxFPS;

      unsigned int _startTicks;
      float _fps;
      float _frameTime;
      float _time;


  };

}
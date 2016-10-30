#include "Timing.h"
#include <SDL.h>

namespace Essengine {

  FPSLimiter::FPSLimiter() { }

  void FPSLimiter::init(float maxFPS, bool limitFPS) {
    setMaxFPS(maxFPS);
    _limitFPS = limitFPS;
  }

  void FPSLimiter::begin() {
    _startTicks = SDL_GetTicks();
  }

  float FPSLimiter::end() {
    float frameTicks = SDL_GetTicks() - _startTicks;

    //limit FPS
    if(_limitFPS && _maxFPS > 0 && 1000.0f / _maxFPS > frameTicks) {
      SDL_Delay(1000.0f / _maxFPS - frameTicks);
    }

    //calculate FPS
    calculateFPS();

    return _fps;
  }

  void FPSLimiter::setMaxFPS(float fps) {
    _maxFPS = fps;
  }

  void FPSLimiter::calculateFPS() {
    static const int NUM_SAMPLES = 10;
    static float frameTimes[NUM_SAMPLES];

    static float prevTicks = SDL_GetTicks();
    static int currentFrame = 0;

    float currentTicks = SDL_GetTicks();

    _frameTime = currentTicks - prevTicks;
    //if it runs too fast, it puts the frame time as 0 so we just wanna approximate to 1
    if(_frameTime == 0) {
      _frameTime = 1;
    }

    frameTimes[currentFrame % NUM_SAMPLES] = _frameTime;

    prevTicks = currentTicks;
    currentFrame++;

    int sampleCount = NUM_SAMPLES;
    if(currentFrame < NUM_SAMPLES) {
      sampleCount = currentFrame;
    }

    float frameTimeAverage = 0;
    for(int i = 0; i < sampleCount; i++) {
      frameTimeAverage += frameTimes[i];
    }

    frameTimeAverage /= sampleCount;

    if(frameTimeAverage > 0) {
      _fps = 1000.0f / frameTimeAverage;
    } else {
      _fps = 0.0f;
    }
  }


}
#pragma once

#include <vector>
#include "TextureAtlas.h"

namespace Ess2D {

  class Animation {
    public:
      Animation();
      ~Animation();

      void setTextureAtlas(TextureAtlas* atlas);
      void setFrames(std::vector<std::string> frames);
      void setPlaybackRate(float playbackRate);
      void setRepeat(bool repeat);
      void setReverse(bool reverse);
      void setAutoStop(bool autoStop);

      void update(float deltaTime);
      void stop();
      void reset();

      std::string getCurrentFrame();
      unsigned int getCurrentFrameNumber();
      unsigned int getFrameCount();
      TextureAtlas* getTextureAtlas();
      bool isReversed();
      bool isPlaying();

    private:
      std::vector<std::string> _frames;
      TextureAtlas* _atlas;
      unsigned int _currentFrame = 0;
      unsigned int _frameCount;
      float _playbackRate;
      float _timeAccumulator;
      bool _repeat = true;
      bool _reverse = false;
      bool _isPlaying = false;
      bool _autoStop = false;

  };

}
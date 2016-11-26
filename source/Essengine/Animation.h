#pragma once

#include <vector>
#include "TextureAtlas.h"

namespace Essengine {

  class Animation {
    public:
      Animation();
      ~Animation();

      void setTextureAtlas(TextureAtlas* atlas);
      void setFrames(std::vector<std::string> frames);
      void setPlaybackRate(float playbackRate);

      void update(float deltaTime);
      void reset();

      std::string getCurrentFrame();
      TextureAtlas* getTextureAtlas();

    private:
      std::vector<std::string> _frames;
      TextureAtlas* _atlas;
      unsigned int _currentFrame = 0;
      unsigned int _frameCount;
      float _playbackRate;
      float _timeAccumulator;

  };

}
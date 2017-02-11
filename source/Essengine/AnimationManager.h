#pragma once
#include "Animation.h"
#include <unordered_map>

namespace Essengine {

  class AnimationManager
  {
    public:
      AnimationManager();
      ~AnimationManager();

      Animation* add(std::string name);
      void play(std::string name);
      void update(float deltaTime);
      Animation* getCurrent();
      std::string getCurrentAnimationName();

    private:
      std::unordered_map<std::string, Animation*> _animations;
      std::string _currentAnimation;

  };

}
#include "AnimationManager.h"
#include "Errors\RuntimeException.cpp"

namespace Essengine {

  AnimationManager::AnimationManager() {}

  AnimationManager::~AnimationManager() {
    auto it = _animations.begin();
    while (it != _animations.end()) {
      delete it->second;
      it++;
    }
  }

  Animation* AnimationManager::add(std::string name) {
    Animation* animation = new Animation();
    _animations.insert(std::make_pair(name, animation));
    return animation;
  }

  void AnimationManager::play(std::string name) {
    auto it = _animations.find(name);
    if (it == _animations.end()) {
      throw new ERuntimeException("Animation " + name + " is not defined");
    }

    _currentAnimation = name;
    _animations.find(_currentAnimation)->second->reset();
  }

  void AnimationManager::update(float deltaTime) {
    _animations.find(_currentAnimation)->second->update(deltaTime);
  }

  Animation* AnimationManager::getCurrent() {
    return _animations.find(_currentAnimation)->second;
  }

}
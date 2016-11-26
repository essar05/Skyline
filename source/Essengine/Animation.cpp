#include "Animation.h"

namespace Essengine {
  
  Animation::Animation() {}
  Animation::~Animation() {}

  void Animation::setTextureAtlas(TextureAtlas* atlas) {
    _atlas = atlas;
  }
  
  void Animation::setFrames(std::vector<std::string> frames) {
    _frames = frames;
    this->_frameCount = _frames.size();
  }

  void Animation::setPlaybackRate(float playbackRate) {
    _playbackRate = playbackRate;
  }

  void Animation::update(float deltaTime) {
    _timeAccumulator += deltaTime;
    int framesElapsed = std::floor(_timeAccumulator / _playbackRate);
    _timeAccumulator -= framesElapsed * _playbackRate;
    _currentFrame = (_currentFrame + framesElapsed) % _frameCount;
  }

  void Animation::reset() {
    _currentFrame = 0;
    _timeAccumulator = 0.0f;
  }

  std::string Animation::getCurrentFrame() {
    return _frames[_currentFrame];
  }

  TextureAtlas* Animation::getTextureAtlas() {
    return _atlas;
  }

}
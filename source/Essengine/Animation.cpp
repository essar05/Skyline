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

  void Animation::setRepeat(bool repeat) {
    _repeat = repeat;
  }

  void Animation::setReverse(bool reverse) {
    _reverse = reverse;
  }

  void Animation::update(float deltaTime) {
    bool isFinished = (_reverse == false && _currentFrame == _frameCount - 1) || (_reverse == true && _currentFrame == 0);
    if (_repeat || !isFinished) {
      _timeAccumulator += deltaTime;
      int framesElapsed = std::floor(_timeAccumulator / _playbackRate);
      _timeAccumulator -= framesElapsed * _playbackRate;
      if (_reverse) {
        _currentFrame = _currentFrame - framesElapsed;
        if (_currentFrame < 0) {
          _currentFrame = _frameCount + _currentFrame;
        }
      } else {
        _currentFrame = (_currentFrame + framesElapsed) % _frameCount;
      }
    }
  }

  void Animation::reset() {
    _currentFrame = 0;
    _timeAccumulator = 0.0f;
    _reverse = false;
  }

  std::string Animation::getCurrentFrame() {
    return _frames[_currentFrame];
  }

  TextureAtlas* Animation::getTextureAtlas() {
    return _atlas;
  }

  unsigned int Animation::getCurrentFrameNumber() {
    return _currentFrame;
  }

  unsigned int Animation::getFrameCount() {
    return _frameCount;
  }

  bool Animation::isReversed() {
    return _reverse;
  }

}
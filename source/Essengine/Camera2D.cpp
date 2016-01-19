#include "Camera2D.h"
#include <iostream>

namespace Essengine {
  Camera2D::Camera2D(): 
    _position(0.0f, 0.0f), 
    _cameraMatrix(1.0f), 
    _scale(1.0f), 
    _doUpdate(true), 
    _screenWidth(640), 
    _screenHeight(480),
    _orthoMatrix(1) {}

  Camera2D::~Camera2D() {}

  void Camera2D::init(int screenWdith, int screenHeight) {
    _screenWidth = screenWdith;
    _screenHeight = screenHeight;

    //build ortho;
    _orthoMatrix = glm::ortho(0.0f, (float) _screenWidth, 0.0f, (float) _screenHeight);
  }

  void Camera2D::update() {
    if(_doUpdate) {
      _cameraMatrix = _orthoMatrix;

      glm::vec3 translation(-_position.x, -_position.y, 0.0f);
      _cameraMatrix = glm::translate(_cameraMatrix, translation);

      glm::vec3 scaling(_scale, _scale, 0.0f);
      _cameraMatrix = glm::scale(_cameraMatrix, scaling);
      _doUpdate = false;
    }
  }

  glm::vec2 Camera2D::getViewportSize() {
    return glm::vec2(_screenWidth / _scale, _screenHeight / _scale);
  }

  glm::vec2 Camera2D::getScaledPosition() {
    return (_position / glm::vec2(_scale, _scale));
  }

}
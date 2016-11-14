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

      //the camera position is in world units so scale it screen. then it's going to get scaled back but it's fine.
      glm::vec2 screenPosition = this->getScreenCoordinates(_position);

      glm::vec3 translation(-screenPosition.x + _screenWidth / 2, -screenPosition.y + _screenHeight / 2, 0.0f);
      _cameraMatrix = glm::translate(_cameraMatrix, translation);

      glm::vec3 scaling(_scale, _scale, 0.0f);
      glm::vec3 zoom(_zoom, _zoom, 0.0f);
      _cameraMatrix = glm::scale(_cameraMatrix, scaling);
      _cameraMatrix = glm::scale(_cameraMatrix, zoom);
      _doUpdate = false;
    }
  }

  //get the camera size within the world
  glm::vec2 Camera2D::getWorldViewportSize() {
    return glm::vec2((_screenWidth / _zoom) / _scale, (_screenHeight / _zoom) / _scale);
  }

  glm::vec2 Camera2D::getViewportSize() {
    return glm::vec2(_screenWidth / _zoom, _screenHeight / _zoom);
  }

  glm::vec2 Camera2D::getPosition() {
    return _position;
  }

  glm::vec2 Camera2D::getWorldCoordinates(glm::vec2 screenCoordinates) {
	  return screenCoordinates / _scale;
  }

  glm::vec2 Camera2D::getScreenCoordinates(glm::vec2 worldCoordinates) {
	  return worldCoordinates * _scale;
  }

  float Camera2D::getWorldScalar(float screenScalar) {
	  return screenScalar / _scale;
  }

  float Camera2D::getScreenScalar(float worldScalar) {
	  return worldScalar * _scale;
  }

}
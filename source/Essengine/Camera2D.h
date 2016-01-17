#pragma once
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

namespace Essengine {
  class Camera2D {
    public:
      Camera2D();
      ~Camera2D();

      void init(int screenWdith, int screenHeight);

      void update();

      void setPosition(const glm::vec2& position) { _position = position; _doUpdate = true; }
      void setScale(float scale) { _scale = scale; _doUpdate = true; }

      glm::vec2 getPosition() { return _position; }
      float getScale() { return _scale; }
      glm::vec2 getViewportSize();

      glm::mat4 getCameraMatrix() { return _cameraMatrix; }

    private:
      int _screenWidth;
      int _screenHeight;

      bool _doUpdate;

      float _scale;
      glm::vec2 _position;
      glm::mat4 _cameraMatrix;
      glm::mat4 _orthoMatrix;

  };
}
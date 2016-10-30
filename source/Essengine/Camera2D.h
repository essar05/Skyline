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
      void setZoom(float zoom) { _zoom = zoom; _doUpdate = true; }

      glm::vec2 getPosition();
      float getScale() { return _scale; }
      float getZoom() { return _zoom; }
      glm::vec2 getWorldViewportSize();
      glm::vec2 getViewportSize();

      glm::vec2 getWorldCoordinates(glm::vec2 screenCoordinates);
      glm::vec2 getScreenCoordinates(glm::vec2 worldCoordinates);
      float getWorldScalar(float screenScalar);
      float getScreenScalar(float worldScalar);

      glm::mat4 getCameraMatrix() { return _cameraMatrix; }

    private:
      int _screenWidth;
      int _screenHeight;

      bool _doUpdate;

      float _scale;
      float _zoom;
      glm::vec2 _position;
      glm::mat4 _cameraMatrix;
      glm::mat4 _orthoMatrix;

  };
}
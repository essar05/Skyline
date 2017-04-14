#pragma once
#include <glm/glm.hpp>
#include <Box2D.h>

class Utils {
  public:
    static glm::vec2 toVec2(const b2Vec2 &vector);
    static b2Vec2 toB2Vec2(const glm::vec2 &vector);
    static glm::vec2 rotatePoint(const glm::vec2& point, const glm::vec2& pivot, float angle);

  private:
    Utils();
};


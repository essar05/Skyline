#include "Utils.h"

glm::vec2 Utils::toVec2(const b2Vec2 &vector) {
  return glm::vec2(vector.x, vector.y);
}

b2Vec2 Utils::toB2Vec2(const glm::vec2 &vector) {
  return b2Vec2(vector.x, vector.y);
}

glm::vec2 Utils::rotatePoint(const glm::vec2& point, const glm::vec2& pivot, float angle) {
  glm::vec2 newPoint;

  newPoint.x = (point.x - pivot.x) * cos(angle) - (point.y - pivot.y) * sin(angle) + pivot.x;
  newPoint.y = (point.x - pivot.x) * sin(angle) + (point.y - pivot.y) * cos(angle) + pivot.y;

  return newPoint;
}
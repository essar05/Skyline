#include "Utils.h"

glm::vec2 Utils::toVec2(const b2Vec2 &vector) {
  return glm::vec2(vector.x, vector.y);
}

b2Vec2 Utils::toB2Vec2(const glm::vec2 &vector) {
  return b2Vec2(vector.x, vector.y);
}
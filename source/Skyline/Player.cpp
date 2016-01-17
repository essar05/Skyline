#include "Player.h"


Player::Player() {
}

Player::Player(int health, int textureId, float width, float height, glm::vec2 position) : Entity(health, textureId, width, height, position) {}

Player::~Player() {
}

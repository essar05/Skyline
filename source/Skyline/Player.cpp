#include "Player.h"

Player::Player() : Player(0, 0.0f, 0.0f, glm::vec2(0.0f, 0.0f)) {
}

Player::Player(int textureId, float width, float height, glm::vec2 position) : Entity(textureId, width, height, position) {
  _projectileSpawner = ProjectileSpawner(_fireRate, 10.0f, 10.0f);
  _projectileSpawner.setSource(1);
}

Player::~Player() {
}

void Player::update(float deltaTime) {
  Entity::update(deltaTime);
  _projectileSpawner.update(deltaTime, _isFiring, _position + glm::vec2(_width / 2, _height), glm::vec2(0.0f, 15.0f));
}
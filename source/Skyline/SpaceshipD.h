#pragma once
#include "Entity.h"
#include "ProjectileSpawner.h"
#include "Game.h"
#include <AnimationManager.h>

class SpaceshipD: public Entity {
public:
  SpaceshipD();
  SpaceshipD(int textureId, glm::vec4 uv, float width, float height, glm::vec2 position, float angle);
  ~SpaceshipD();

protected:
  void draw() override;
  bool update(float deltaTime) override;
  virtual void createFixtures() override;
  virtual void die() override;

  void setIsFiring(bool isFiring) { _isFiring = isFiring; }

  int getType() override { return ET_SPACESHIP_D; }

private:
  void initAnimations();
  void initThruster();
  void initProjectileSpawners();

  float _fireRate = 8.0f;
  bool _isFiring = true;
  ProjectileSpawner _projectileSpawnerLeft;
  ProjectileSpawner _projectileSpawnerRight;
  Ess2D::AnimationManager* _animationManager;
  Ess2D::AnimationManager* _thrusterAnimationManager;
  Ess2D::AnimationManager* _muzzleLeftAnimationManager;
  Ess2D::AnimationManager* _muzzleRightAnimationManager;

  glm::vec2 _projectileSpawnerLeftPosition;
  glm::vec2 _projectileSpawnerRightPosition;

  float _horizontalScaleFactor = 1.0f;

  float _thrusterHeight = 50.0f;
  float _thrusterWidth = 40.2f;

  glm::vec2 _muzzleSize = glm::vec2(0.6f, 0.6f);
};


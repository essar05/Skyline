#include "LevelDecoration.h"
#include "Game.h"

LevelDecoration::LevelDecoration(GLuint textureId, float x, float y, float width, float height, float scale) {
  Game* game = Game::GetInstance();

  _textureId = textureId;
  _x = game->getMainCamera()->getWorldScalar(x);
  _y = game->getMainCamera()->getWorldScalar(y);
  _width = game->getMainCamera()->getWorldScalar(width * scale);
  _height = game->getMainCamera()->getWorldScalar(height * scale);
}

LevelDecoration::~LevelDecoration() {}

void LevelDecoration::setScale(float scale) {
  _scale = scale;
}

float LevelDecoration::getX() {
  return _x;
}

float LevelDecoration::getY() {
  return _y;
}

float LevelDecoration::getWidth() {
  return _width;
}

float LevelDecoration::getHeight() {
  return _height;
}

GLuint LevelDecoration::getTextureId() {
  return _textureId;
}
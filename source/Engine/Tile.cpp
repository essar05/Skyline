#include "Tile.h"

using namespace Engine;

Tile::Tile() {
  this->id = 0;
  this->x = 0.0f;
  this->y = 0.0f;
}

Tile::Tile(int id, float x, float y) {
  this->id = id;
  this->x = x;
  this->y = y;
}

Tile::~Tile() {}

int Tile::GetId() {
  return this->id;
}

void Tile::SetId(int id) {
  this->id = id;
}

float Tile::GetX() {
  return this->x;
}

float Tile::GetY() {
  return this->y;
}

void Tile::SetX(float x) {
  this->x = x;
}

void Tile::SetY(float y) {
  this->y = y;
}

void Tile::Translate(float dx, float dy) {
  this->x = dx;
  this->y = dy;
}
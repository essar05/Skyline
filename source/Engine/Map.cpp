#include "Map.h"

using namespace Engine;

Map::Map() {

}

Map::~Map() {

}

int Map::GetWidth() {
  return this->width;
}

int Map::GetHeight() {
  return this->height;
}

int Map::GetLayersCount() {
  return this->layersCount;
}

void Map::SetWidth(int aWidth) {
  this->width = aWidth;
}

void Map::SetHeight(int aHeight) {
  this->height = aHeight;
}

void Map::SetLayersCount(int aLayersCount) {
  this->layersCount = aLayersCount;
}
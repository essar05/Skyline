#include "Map.h"
#include <math.h>
#include <stdio.h>
#include <ctime>

using namespace Engine;

Map::Map(int aTileWidth, int aTileHeight) {
	this->tileWidth = aTileWidth;
	this->tileHeight = aTileHeight;
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

int Map::GetOffsetWidth() {
	return this->offsetWidth;
}

int Map::GetOffsetHeight() {
	return this->offsetHeight;
}

int Map::GetOffsetI() {
	return this->offsetI;
}

int Map::GetOffsetJ() {
	return this->offsetJ;
}

int Map::GetTileWidth() {
	return this->tileWidth;
}

int Map::GetTileHeight() {
	return this->tileHeight;
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

void Map::ComputeVisibleTiles(int anXOffset, int aYOffset, int aScreenWidth, int aScreenHeight) {
	float dx, dy;
	for (int i = 0; i < this->height; i++) {
		for (int j = 0; j < this->width; j++) {
			dx = static_cast<float>(j * this->tileWidth + ((i) % 2) * (this->tileWidth / 2));
			dy = static_cast<float>(i * (this->tileHeight / 2));
			this->Tiles[0][i][j]->Translate(dx, dy);
		}
	}
}

void Map::ComputeTilePositions(int anXOffset, int aYOffset, int aScreenWidth, int aScreenHeight) {
	/*this->offsetWidth = static_cast<int>(
		floor(
		(float)(aScreenWidth / this->tileWidth)
		)
		) + 2;
	this->offsetHeight = static_cast<int>(
		ceil(
		(float)(aScreenHeight / (this->tileHeight / 2))
		)
		) + 2;
	this->offsetI = static_cast<int>(floor((float)(-aYOffset / (this->tileHeight / 2))));
	this->offsetJ = static_cast<int>(floor((float)(
		anXOffset / (this->tileWidth)
		)));
	float offsetX = -fmod((float)anXOffset, (float)(this->tileWidth));
	float offsetY = -fmod((float)-aYOffset, (float)(this->tileHeight / 2));

	if (this->offsetI < 0) {
		this->offsetI = 0;
	}

	if (this->offsetJ < 0) {
		this->offsetJ = 0;
	}

	/*printf("w: %d", this->offsetWidth);
	printf("h: %d", this->offsetHeight);
	printf("i: %d", this->offsetI);
	printf("j: %d", this->offsetJ);*/
	/*
	float dx, dy;
	for (int i = 0; i < this->offsetHeight; i++) {
		for (int j = 0; j < this->offsetWidth; j++) {
			dx = static_cast<float>(j * this->tileWidth + ((i + this->offsetI) % 2) * (this->tileWidth / 2)) + offsetX;
			dy = static_cast<float>(i * (this->tileHeight / 2)) + offsetY;
			this->Tiles[0][i + this->offsetI][j + this->offsetJ]->Translate(dx, dy);
		}
	}*/

}
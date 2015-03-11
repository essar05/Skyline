#pragma once
#include "Tile.h"
namespace Engine {

  class Map {
    public:
      Map();
	  Map(int aTileWidth, int aTileHeight);
      ~Map();
      Tile**** Tiles;
      bool** Walkable;
      bool** Passable;
      int GetWidth();
      int GetHeight();
      int GetLayersCount();
      void SetWidth(int aWidth);
      void SetHeight(int aHeight);
      void SetLayersCount(int aLayersCount);
	  void ComputeTilePositions(int anXOffset, int aYOffset, int aScreenWidth, int aScreenHeight);
	  int GetOffsetWidth();
	  int GetOffsetHeight();
	  int GetOffsetI();
	  int GetOffsetJ();
    private:
      int width;
      int height;
      int layersCount;
	  int tileWidth;
	  int tileHeight;
	  int offsetWidth;
	  int offsetHeight;
	  int offsetI;
	  int offsetJ;
  };
};


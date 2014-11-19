#pragma once
namespace Engine {
  class Map {
    public:
      Map();
      ~Map();
      int*** Tiles;
      bool** Walkable;
      bool** Passable;
      int GetWidth();
      int GetHeight();
      int GetLayersCount();
      void SetWidth(int aWidth);
      void SetHeight(int aHeight);
      void SetLayersCount(int aLayersCount);
    private:
      int width;
      int height;
      int layersCount;
  };
};


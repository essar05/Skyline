#pragma once
namespace Engine {
  class Tile {
    public:
      Tile();
      Tile(int id, float x, float y);
      ~Tile();
      int GetId();
      void SetId(int id);
      float GetX();
      float GetY();
      void SetX(float x);
      void SetY(float y);
      void Translate(float dx, float dy);
    private:
      float x;
      float y;
      int id;
  };
};
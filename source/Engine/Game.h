#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <glew.h>
#include "RuntimeException.cpp"
#include "SDLW.h"
#include "GLW.h"
#include "Shader.h"
#include "Renderer.h"
#include "Map.h"
#include "MapParser.h"

namespace Engine {
  class Renderer;
  class Game {
    public:
      void Boot();
      static Game* GetInstance();
      void Destroy();
      void Render();
      void Run();

    private:
      Game();
      ~Game();
      void doRun(SDL_Event* anEvent);
      void generateTempMap();

      static Game* instance;
      Renderer* renderer;
      Map* map;
      MapParser* parser;
  };

};

#endif // GAME_H

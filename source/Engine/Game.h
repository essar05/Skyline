#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <glew.h>
#include "RuntimeException.cpp"
#include "SDLW.h"
#include "GLW.h"
#include "Shader.h"
#include "Renderer.h"

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

      static Game* instance;
      Renderer* renderer;
  };

};

#endif // GAME_H

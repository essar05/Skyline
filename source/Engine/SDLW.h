#ifndef SDLW_H
#define SDLW_H

#include <SDL.h>

namespace Engine {
  class SDLW {
    public:
      //Methods
      SDLW();
      static SDL_Window* CreateWindow_(const char* aTitle, int X, int Y, int aWidth, int aHeight, Uint32 someFlags);
      static void DestroyWindow_(SDL_Window* aWindow);
      static const char* GetError();
      static SDL_GLContext GLCreateContext(SDL_Window* aWindow);
      static void GLEWInit();
      static void GLSetSwapInterval(int anInterval);
      static void GLSwapWindow(SDL_Window* aWindow);
      static void Init(Uint32 someFlags);
      static int PollEvent(SDL_Event* anEvent);
      static void Quit();
      static int SetGLAttribute(SDL_GLattr anAttribute, int aValue);
      static void StartTextInput();
      static void StopTextInput();
    protected:
    private:
  };
}
#endif // SDLW_H

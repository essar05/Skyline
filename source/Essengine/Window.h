#pragma once

#include <SDL.h>
#include <glew.h>
#include <string>
#include "RuntimeException.cpp"

namespace Essengine {

  enum WindowMode {
    INVISIBLE = 0x1,
    FULLSCREEN = 0x2,
    BORDERLESS = 0X4,
    FULLSCREEN_DESKTOP = 0x8
  };

  class Window {
    public:
      Window(std::string windowName, int width, int height, unsigned int windowModes);
      ~Window();

      void SwapBuffer();
      void SetVSync(int vSync);

      int GetWidth() { return _width; }
      int GetHeight() { return _height; }
      int GetMonitorRefreshRate();

    private:
      int _height;
      int _width;
      SDL_Window* _sdlWindow;

  };

}
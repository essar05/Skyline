#include "Window.h"


namespace Ess2D {

  Window::Window(std::string windowName, int width, int height, unsigned int windowModes): _width(width), _height(height) {
    unsigned int windowFlags = SDL_WINDOW_OPENGL;

    if (windowModes & WindowMode::INVISIBLE) {
      windowFlags |= SDL_WINDOW_HIDDEN;
    }
    if (windowModes & WindowMode::FULLSCREEN) {
      windowFlags |= SDL_WINDOW_FULLSCREEN;
    }
    if (windowModes & WindowMode::FULLSCREEN_DESKTOP) {
      windowFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    }
    if (windowModes & WindowMode::BORDERLESS) {
      windowFlags |= SDL_WINDOW_BORDERLESS;
    }

    _sdlWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _width, _height, windowFlags);
    if (_sdlWindow == NULL) {
      throw Ess2D::ERuntimeException(SDL_GetError());
    }

    //Create OpenGL Context;
    SDL_GLContext glContext = SDL_GL_CreateContext(_sdlWindow);
    if (glContext == NULL) {
      throw Ess2D::ERuntimeException("GL Context creation failed");
    }

    GLenum error = glewInit();
    if (error != GLEW_OK) {
      throw Ess2D::ERuntimeException("GLEW initialization failed");
    }

    std::printf("OpenGL Version: %s\n", glGetString(GL_VERSION));

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    //Set VSYNC to disabled
    SDL_GL_SetSwapInterval(0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  }

  Window::~Window() {
  }

  int Window::GetMonitorRefreshRate() {
    SDL_DisplayMode mode = { SDL_PIXELFORMAT_UNKNOWN, 0, 0, 0, 0 };
    int windowDisplayIndex = SDL_GetWindowDisplayIndex(this->_sdlWindow);
    if(windowDisplayIndex < 0) {
      std::string sdlError = SDL_GetError();
      throw Ess2D::ERuntimeException("Error in Window::GetMonitorRefreshRate: " + sdlError);
    }

    if(SDL_GetDisplayMode(windowDisplayIndex, 0, &mode) < 0) {
      std::string sdlError = SDL_GetError();
      throw Ess2D::ERuntimeException("Error in Window::GetMonitorRefreshRate: " + sdlError);
    }

    return mode.refresh_rate;
  }

  void Window::SetVSync(int vSync) {
    if(vSync != 0 && vSync != 1 && vSync != -1) {
      throw Ess2D::ERuntimeException("Incorrect value passed to Window::SetVSync");
    }

    SDL_GL_SetSwapInterval(vSync);
  }

  void Window::SwapBuffer() {
    SDL_GL_SwapWindow(this->_sdlWindow);
  }

}
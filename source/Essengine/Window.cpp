#include "Window.h"


namespace Essengine {

  Window::Window(std::string windowName, int width, int height, unsigned int windowModes): _width(width), _height(height) {
    unsigned int windowFlags = SDL_WINDOW_OPENGL;

    if (windowModes & WindowMode::INVISIBLE) {
      windowFlags |= SDL_WINDOW_HIDDEN;
    }
    if (windowModes & WindowMode::FULLSCREEN) {
      windowFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    }
    if (windowModes & WindowMode::BORDERLESS) {
      windowFlags |= SDL_WINDOW_BORDERLESS;
    }

    _sdlWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _width, _height, windowFlags);
    if (_sdlWindow == NULL) {
      throw Essengine::ERuntimeException(SDL_GetError());
    }

    //Create OpenGL Context;
    SDL_GLContext glContext = SDL_GL_CreateContext(_sdlWindow);
    if (glContext == NULL) {
      throw Essengine::ERuntimeException("GL Context creation failed");
    }

    GLenum error = glewInit();
    if (error != GLEW_OK) {
      throw Essengine::ERuntimeException("GLEW initialization failed");
    }

    std::printf("OpenGL Version: %s\n", glGetString(GL_VERSION));

    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);

    //Set VSYNC
    SDL_GL_SetSwapInterval(-1);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  }

  Window::~Window() {
  }

  void Window::SwapBuffer() {
    SDL_GL_SwapWindow(this->_sdlWindow);
  }

}
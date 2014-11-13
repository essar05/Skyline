#include "SDLW.h"
#include "RuntimeException.cpp"
#include <SDL.h>
#include <glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>
#include <stdio.h>
#include <string>

using namespace Engine;

SDLW::SDLW() {
  throw ERuntimeException("You cannot directly initialize SDL Wrapper class (Engine::SDLW)");
}

SDL_Window* SDLW::CreateWindow_(const char* aTitle, int X, int Y, int aWidth, int aHeight, Uint32 someFlags) {
  SDL_Window* theWindow = SDL_CreateWindow(aTitle, X, Y, aWidth, aHeight, someFlags);
  if(theWindow == NULL) {
    throw ERuntimeException(SDLW::GetError());
  }
  return theWindow;
}

void SDLW::DestroyWindow_(SDL_Window* aWindow) {
  SDL_DestroyWindow(aWindow);
}

const char* SDLW::GetError() {
  return SDL_GetError();
}

SDL_GLContext SDLW::GLCreateContext(SDL_Window* aWindow) {
  SDL_GLContext theContext = SDL_GL_CreateContext(aWindow);
  if(theContext == NULL) {
    throw ERuntimeException(SDLW::GetError());
  }
  return theContext;
}

void SDLW::GLEWInit() {
  glewExperimental = GL_TRUE;
  GLenum glewError = glewInit();
  if(glewError != GLEW_OK) {
    throw ERuntimeException(reinterpret_cast<const char*>(glewGetErrorString(glewError)));
  }
}

void SDLW::GLSetSwapInterval(int anInterval) {
  if(SDL_GL_SetSwapInterval( anInterval ) < 0) {
    throw ERuntimeException(SDLW::GetError());
  }
}

void SDLW::GLSwapWindow(SDL_Window* aWindow) {
  SDL_GL_SwapWindow(aWindow);
}

void SDLW::Init(Uint32 someFlags) {
  if(SDL_Init(someFlags) < 0) {
    throw ERuntimeException(SDLW::GetError());
  }
}

int SDLW::PollEvent(SDL_Event* anEvent) {
  return SDL_PollEvent(anEvent);
}

void SDLW::Quit() {
  SDL_Quit();
}

int SDLW::SetGLAttribute(SDL_GLattr anAttribute, int aValue) {
  return SDL_GL_SetAttribute(anAttribute, aValue);
}

void SDLW::StartTextInput() {
  SDL_StartTextInput();
}

void SDLW::StopTextInput() {
  SDL_StopTextInput();
}

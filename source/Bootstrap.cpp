#include <SDL.h>
#include <glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>
#include <stdio.h>
#include <string>
#include "RuntimeException.cpp"
#include "Game.h"

using namespace Engine;

int main( int argc, char* args[] ) {
  CGame* theInstance = CGame::GetInstance();

  try {
    theInstance->Boot();

    theInstance->Run();

    theInstance->Destroy();
  } catch(ERuntimeException& anException) {
    printf(anException.what());
  }

	return 0;
}

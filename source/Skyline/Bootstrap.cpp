#define _CRTDBG_MAP_ALLOC
#include <RuntimeException.cpp>
#include "Game.h"

int main(int argc, char* args[]) {
  Game* theInstance = Game::GetInstance();

  try {
    theInstance->Boot();

    theInstance->Run();

    theInstance->Destroy();
  }
  catch (Essengine::ERuntimeException& anException) {
    std::cout << anException.getMessage() << std::endl;
  }

  return 0;
}

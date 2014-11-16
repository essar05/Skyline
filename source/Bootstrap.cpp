#include "RuntimeException.cpp"
#include "Game.h"

using namespace Engine;

int main(int argc, char* args[]) {
  Game* theInstance = Game::GetInstance();

  try {
    theInstance->Boot();

    theInstance->Run();

    theInstance->Destroy();
  } catch(ERuntimeException& anException) {
    printf(anException.what());
  }

  return 0;
}

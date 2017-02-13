#define _CRTDBG_MAP_ALLOC
#include <RuntimeException.cpp>
#include "Game.h"

int main(int argc, char* args[]) {
  Game* game = Game::GetInstance();

  try {
    game->Boot();

    game->Run();

    game->Destroy();
  }
  catch (Ess2D::ERuntimeException& anException) {
    std::cout << anException.getMessage() << std::endl;
  }

  return 0;
}

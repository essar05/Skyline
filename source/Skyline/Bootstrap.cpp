#define _CRTDBG_MAP_ALLOC
#include <RuntimeException.cpp>
#include "Game.h"

int main(int argc, char* args[]) {
  Game* game = Game::GetInstance();

  try {
    game->boot();
    game->run();
    game->destroy();
  }
  catch (Ess2D::ERuntimeException& anException) {
    std::cout << anException.getMessage() << std::endl;
  }

  return 0;
}

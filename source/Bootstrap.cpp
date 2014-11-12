#include "RuntimeException.cpp"
#include "Engine/Game.h"

using namespace Pixeltwo;

int main( int argc, char* args[] ) {
  CGame* theInstance = new CGame;

  try {
    theInstance->boot();
  } catch(ERuntimeException& anException) {
    printf(anException.what());
  }
  theInstance->run();

  delete theInstance;

	return 0;
}

#include "ScreenManager.h"

namespace Ess2D {

  ScreenManager::ScreenManager() {

  }

  ScreenManager::~ScreenManager() {
    for (size_t i = 0; i < _screens.size(); i++) {
      _screens[i]->destroy();
     // delete _screens[i];
    }
  }

  IGameScreen* ScreenManager::moveNext() {
    IGameScreen* currentScreen = getCurrent();
    if (currentScreen->getNextScreenIndex() != SCREEN_INDEX_NO_SCREEN) {
      _currentScreenIndex = currentScreen->getNextScreenIndex();
    }
    return getCurrent();
  }

  IGameScreen* ScreenManager::movePrevious() {
    IGameScreen* currentScreen = getCurrent();
    if (currentScreen->getPreviousScreenIndex() != SCREEN_INDEX_NO_SCREEN) {
      _currentScreenIndex = currentScreen->getPreviousScreenIndex();
    }
    return getCurrent();
  }

  void ScreenManager::setScreen(int screenIndex) {
    _currentScreenIndex = screenIndex;
  }

  void ScreenManager::addScreen(IGameScreen* newScreen) {
    newScreen->_screenIndex = _screens.size();
    _screens.push_back(newScreen);
    newScreen->build();
  }

  IGameScreen* ScreenManager::getCurrent() {
    return _screens[_currentScreenIndex];
  }

}
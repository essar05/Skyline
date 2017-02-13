#include "InputManager.h"

namespace Ess2D {

  InputManager::InputManager() {
  }

  InputManager::~InputManager() {
  }

  void InputManager::pressKey(unsigned int keyId) {
    _keyMap[keyId] = true;
  }

  void InputManager::releaseKey(unsigned int keyId) {
    _keyMap[keyId] = false;
  }

  bool InputManager::isKeyDown(unsigned int keyId) {
    auto it = _keyMap.find(keyId);
    if(it == _keyMap.end()) {
      return false;
    }
    return it->second;
  }

}
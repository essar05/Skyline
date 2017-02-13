#pragma once

#include <unordered_map>

namespace Ess2D {
  class InputManager {
    public:
      InputManager();
      ~InputManager();

      void pressKey(unsigned int keyId);
      void releaseKey(unsigned int keyId);

      bool isKeyDown(unsigned int keyId);

    private:
      std::unordered_map<unsigned int, bool> _keyMap;

  };
}
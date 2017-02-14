#pragma once
#include <fmod_studio.hpp>
#include <fmod.hpp>

namespace Ess2D {

  class AudioManager {
    public:
      AudioManager();
      ~AudioManager();

      void update();

    private:
      FMOD::Studio::System* _fmod;
      FMOD::System* _fmodLowLevel;

      void _errorCheck(FMOD_RESULT result);
  };

}
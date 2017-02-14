#include "AudioManager.h"
#include <fmod_errors.h>
#include "Errors\RuntimeException.cpp"

namespace Ess2D {

  AudioManager::AudioManager() {
    FMOD_RESULT result;

    result = FMOD::Studio::System::create(&_fmod);
    _errorCheck(result);

    result = _fmod->initialize(512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0);
    _errorCheck(result);

    result = _fmod->getLowLevelSystem(&_fmodLowLevel);
    _errorCheck(result);
  }

  AudioManager::~AudioManager() {
    _errorCheck(_fmod->unloadAll());
    _errorCheck(_fmod->release());
  }

  void AudioManager::update() {
    _errorCheck(_fmod->update());
  }

  void AudioManager::_errorCheck(FMOD_RESULT result) {
    if (result != FMOD_OK) {
      throw ERuntimeException("FMOD Error: " + std::string(FMOD_ErrorString(result)));
    }
  }

}
#include "AudioManager.h"
#include <fmod_errors.h>
#include "Errors\RuntimeException.cpp"

namespace Ess2D {

  AudioManager::AudioManager() {
    _errorCheck( FMOD::Studio::System::create(&_fmod) );
    _errorCheck( _fmod->initialize(512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0) );
    _errorCheck( _fmod->getLowLevelSystem(&_fmodLowLevel) );
  }

  AudioManager::~AudioManager() {
    _errorCheck(_fmod->unloadAll());
    _errorCheck(_fmod->release());
  }

  void AudioManager::loadBank(const std::string &bankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags) {
    auto it = _banks.find(bankName);
    if (it != _banks.end()) {
      return;
    }

    FMOD::Studio::Bank* bank = nullptr;
    _errorCheck( _fmod->loadBankFile(bankName.c_str(), flags, &bank) );
    if (bank) {
      _banks[bankName] = bank;
    }
  }

  void AudioManager::loadEvent(const std::string &eventName) {
    auto it = _events.find(eventName);
    if (it != _events.end()) {
      return;
    }

    FMOD::Studio::EventDescription* eventDescription = nullptr;
    _errorCheck( _fmod->getEvent(eventName.c_str(), &eventDescription) );
    if (eventDescription) {
      _events[eventName] = eventDescription;

      FMOD::Studio::EventInstance* eventInstance = nullptr;
      _errorCheck( eventDescription->createInstance(&eventInstance) );
      if (eventInstance) {
        _eventInstances[eventName] = eventInstance;
      }
    }
  }

  void AudioManager::playEvent(const std::string &eventName, bool newInstance) {
    auto it = _events.find(eventName);
    if (it == _events.end()) {
      this->loadEvent(eventName);
      it = _events.find(eventName);
      if (it == _events.end()) {
        return;
      }
    }

    FMOD::Studio::EventInstance* eventInstance = nullptr;
    if (newInstance) {
      _errorCheck( it->second->createInstance(&eventInstance) );
      _errorCheck( eventInstance->start() );
      _errorCheck( eventInstance->release() );
    } else {
      auto itInstance = _eventInstances.find(eventName);
      if (itInstance == _eventInstances.end()) {
        _errorCheck( it->second->createInstance(&eventInstance) );
        _eventInstances[eventName] = eventInstance;
      } else {
        eventInstance = itInstance->second;
      }

      _errorCheck( eventInstance->start() );
    }
  }

  void AudioManager::stopEvent(const std::string &eventName, bool immediate) {
    auto it = _eventInstances.find(eventName);
    if (it == _eventInstances.end()) {
      return;
    }

    FMOD_STUDIO_STOP_MODE stopMode = immediate ? FMOD_STUDIO_STOP_IMMEDIATE : FMOD_STUDIO_STOP_ALLOWFADEOUT;
    _errorCheck( it->second->stop(stopMode) );
  }

  void AudioManager::update() {
    _errorCheck( _fmod->update() );
  }

  FMOD::Studio::EventInstance* AudioManager::getEvent(const std::string &eventName) {
    auto it = _eventInstances.find(eventName);
    if(it == _eventInstances.end()) {
      return nullptr;
    }

    return it->second;
  }

  void AudioManager::setMasterVolume(float volume) {
    FMOD::Studio::Bus* masterBus;
    _errorCheck(_fmod->getBus("bus:/", &masterBus));

    if(volume > 1.0f) {
      volume = 1.0f;
    }

    _errorCheck(masterBus->setVolume(volume));
  }

  void AudioManager::_errorCheck(FMOD_RESULT result) {
    if (result != FMOD_OK) {
      throw ERuntimeException("FMOD Error: " + std::string(FMOD_ErrorString(result)));
    }
  }

}
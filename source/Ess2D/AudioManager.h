#pragma once
#include <fmod_studio.hpp>
#include <fmod.hpp>
#include <map>

namespace Ess2D {

  class AudioManager {
    public:
      AudioManager();
      ~AudioManager();

      void update();
      void loadBank(const std::string &bankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags);
      void loadEvent(const std::string &eventName);
      void playEvent(const std::string &eventName, bool newInstance = false);
      void stopEvent(const std::string &eventName, bool immediate);
      FMOD::Studio::EventInstance* getEvent(const std::string &eventName);
      void setMasterVolume(float volume);
      
      void _errorCheck(FMOD_RESULT result);

    private:
      FMOD::Studio::System* _fmod;
      FMOD::System* _fmodLowLevel;

      std::map <std::string, FMOD::Studio::Bank*> _banks;
      std::map <std::string, FMOD::Studio::EventDescription*> _events;
      std::map <std::string, FMOD::Studio::EventInstance*> _eventInstances;

  };

}
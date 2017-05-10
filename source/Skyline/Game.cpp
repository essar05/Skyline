#define NOMINMAX

#include "Game.h"
/*#include <iostream>
#include <TextureCache.h>
#include <Texture2D.h>
#include <windows.h>
#include <algorithm>
#include "Entity.h"
#include "Utils.h"
#include <chrono>*/

Game::Game() {
  _title = "Skyline";

  _maxFPS = 60.0f;
  _debugMode = false;
  _limitFPS = false;
  _vSync = true;

  _width = 1280.0f;
  _height = 720.0f;
  _windowMode = Ess2D::WindowMode::NORMAL;

  loadConfig();
}

Game::~Game() {}

Game* Game::instance = NULL;

void Game::addScreens() {
  _mainMenuScreen = new MainMenuScreen;
  _gameplayScreen = new GameplayScreen;

  _screenManager->addScreen(_mainMenuScreen);
  _screenManager->addScreen(_gameplayScreen);
  _screenManager->setScreen(_mainMenuScreen->getIndex());
}

void Game::onInit() {
  _audioManager.loadBank("Sounds/Master Bank.bank", FMOD_STUDIO_LOAD_BANK_NORMAL);
  _audioManager.loadBank("Sounds/Master Bank.strings.bank", FMOD_STUDIO_LOAD_BANK_NORMAL);
  _audioManager.loadBank("Sounds/Music.bank", FMOD_STUDIO_LOAD_BANK_NORMAL);
  _audioManager.loadBank("Sounds/Effects.bank", FMOD_STUDIO_LOAD_BANK_NORMAL);

  //temporarily disabled, cause it's very annoying when debugging (perhaps when playing too, lol)
  _audioManager.playEvent("event:/music/heavyrain_david");

  _audioManager.setMasterVolume(_volume);
}

void Game::onExit() {
  delete _gameplayScreen;
  delete _mainMenuScreen;
}

void Game::onUpdate() {
  _audioManager.update();
}

void Game::loadConfig() {
  std::ifstream fileStream("config.json");
  std::string jsonData;

  if(!fileStream.is_open()) {
    return;
  }

  fileStream.seekg(0, std::ios::end);
  jsonData.reserve((unsigned int)fileStream.tellg());
  fileStream.seekg(0, std::ios::beg);

  jsonData.assign((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());

  rapidjson::Document document;
  document.Parse(jsonData.c_str());

  if(!document.IsObject()) return;

  if(document.HasMember("width") && document["width"].IsInt() && document["width"].GetInt() >= 800) {
    _width = (float)document["width"].GetInt();
  }

  if(document.HasMember("height") && document["height"].IsInt() && document["height"].GetInt() >= 600) {
    _height = (float)document["height"].GetInt();
  }

  if(document.HasMember("fullscreen") && document["fullscreen"].IsBool()) {
    if(document["fullscreen"].GetBool()) {
      _windowMode = Ess2D::WindowMode::FULLSCREEN;
    }
  }

  if(document.HasMember("vSync") && document["vSync"].IsBool()) {
    this->_vSync = document["vSync"].GetBool();
  }

  if(document.HasMember("masterVolume") && document["masterVolume"].IsNumber()) {
    _volume = document["masterVolume"].GetDouble();
  }
}

Game* Game::GetInstance() {
  if (instance == NULL) {
    instance = new Game();
  }
  true;
  return instance;
}

Ess2D::AudioManager* Game::getAudioManager() {
  return &_audioManager;
}

GameplayScreen* Game::getGameplayScreen() {
  return _gameplayScreen;
}

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

Game::Game() {}

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
}

void Game::onExit() {
  delete _gameplayScreen;
  delete _mainMenuScreen;
}

void Game::onUpdate() {
  _audioManager.update();
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

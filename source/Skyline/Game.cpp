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
  _gameplayScreen = new GameplayScreen;
  _screenManager->addScreen(_gameplayScreen);
  _screenManager->setScreen(_gameplayScreen->getIndex());
}

void Game::onInit() {
  _audioManager.loadBank("Sounds/Master Bank.bank", FMOD_STUDIO_LOAD_BANK_NORMAL);
  _audioManager.loadBank("Sounds/Master Bank.strings.bank", FMOD_STUDIO_LOAD_BANK_NORMAL);
  _audioManager.loadBank("Sounds/Music.bank", FMOD_STUDIO_LOAD_BANK_NORMAL);
  _audioManager.loadBank("Sounds/Effects.bank", FMOD_STUDIO_LOAD_BANK_NORMAL);

  //temporarily disabled, cause it's very annoying when debugging (perhaps when playing too, lol)
  /*_audioManager.playEvent("event:/music/heavyrain_david");
  _audioManager.playEvent("event:/effects/ambience_ship");*/
}

void Game::onExit() {
  delete _gameplayScreen;
}

void Game::onUpdate() {
  _audioManager.update();
}

/*
void Game::initUI() {
  _gui.init("GUI");
  _gui.loadScheme("TaharezLook.scheme");
  _gui.loadScheme("AlfiskoSkin.scheme");
  _gui.setFont("OpenSans-Bold");

  CEGUI::PushButton* button = static_cast<CEGUI::PushButton*> (
    _gui.createWidget("AlfiskoSkin/Button", glm::vec4(0.5f, 0.5f, 0.1f, 0.05f), glm::vec4(0.0f), "Button")
    );
  button->setWantsMultiClickEvents(false);
  button->setText("Test GUI");

  _gui.setMouseCursor("TaharezLook/MouseArrow");
  _gui.showMouseCursor();

  CEGUI::Editbox* editbox = static_cast<CEGUI::Editbox*> (
    _gui.createWidget("AlfiskoSkin/Editbox", glm::vec4(0.2f, 0.2f, 0.1f, 0.05f), glm::vec4(0.0f), "Editbox")
    );
}*/

Game* Game::GetInstance() {
  if (instance == NULL) {
    instance = new Game();
  }
  return instance;
}

Ess2D::AudioManager* Game::getAudioManager() {
  return &_audioManager;
}

GameplayScreen* Game::getGameplayScreen() {
  return _gameplayScreen;
}

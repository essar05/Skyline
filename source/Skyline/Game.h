#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <GL/glew.h>
#include <iostream>
#include <IGame.h>
#include <AudioManager.h>
#include "GameplayScreen.h"
#include "MainMenuScreen.h"

class GameplayScreen;
class MainMenuScreen;

class Game : public Ess2D::IGame {
  public:
	  static Game* GetInstance();
    ~Game();

    virtual void addScreens() override;
    virtual void onInit() override;
    virtual void onExit() override;

    Ess2D::AudioManager* getAudioManager();
    GameplayScreen* getGameplayScreen();

  protected:
    Game();
    virtual void onUpdate() override;
    void loadConfig();

    float _volume = 1.0f;

    Ess2D::AudioManager _audioManager;

  private:
    static Game* instance;
    GameplayScreen* _gameplayScreen;
    MainMenuScreen* _mainMenuScreen;

};

#endif // GAME_H

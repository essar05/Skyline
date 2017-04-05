#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <GL/glew.h>
#include <iostream>
#include <IGame.h>
#include <AudioManager.h>
#include "GameplayScreen.h"


class GameplayScreen;

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

    //settings
    float _maxFPS = 60.0f;
    bool _debugMode = false;
    bool _limitFPS = false;
    bool _vSync = 0;

    float _width = 1280.0f;
    float _height = 720.0f;
    std::string _title = "Skyline 1.0";
    Ess2D::WindowMode _windowMode = Ess2D::WindowMode::NORMAL;
    Ess2D::AudioManager _audioManager;

  private:
    static Game* instance;
    GameplayScreen* _gameplayScreen;

};

#endif // GAME_H

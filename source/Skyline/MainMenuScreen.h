#pragma once
#include <IGameScreen.h>
#include <GUI.h>
#include "ScreenIndices.h"
#include "Game.h"

class Game;

class MainMenuScreen : public Ess2D::IGameScreen {
  public:
    MainMenuScreen();
    ~MainMenuScreen();

    virtual int getNextScreenIndex() const;
    virtual int getPreviousScreenIndex() const;

    //called at beggining and end of application
    virtual void build();
    virtual void destroy();

    // called when a screen enters and exists focus
    virtual void onEntry();
    virtual void onExit();

    // called in the main game loop
    virtual void update(float deltaTime, int simulationSteps);
    virtual void draw();

    bool onBtnStartGameClicked(const CEGUI::EventArgs &e);
    bool onBtnCreditsClicked(const CEGUI::EventArgs &e);
    bool onBtnExitClicked(const CEGUI::EventArgs &e);

  private:
    void processInput(float deltaTime);

    Game* _game = nullptr;
    Ess2D::GUI _gui;
};


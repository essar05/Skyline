#pragma once

#include <IGameScreen.h>
#include "Game.h"
#include <SpriteBatch.h>
#include <Camera2D.h>
#include <TextureCache.h>
#include <GUI.h>
#include "Level.h"
#include "ProjectileManager.h"
#include "EntityManager.h"
#include "SceneRenderer.h"
#include "FBORenderer.h"
#include "PostProcessing.h"
#include "ScreenIndices.h"
#include "ParticleManager.h"
#include "PowerupManager.h"

class Level;
class ProjectileManager;
class PostProcessing;

class GameplayScreen : public Ess2D::IGameScreen {
  public:
    GameplayScreen();
    ~GameplayScreen();

    virtual int getNextScreenIndex() const override;
    virtual int getPreviousScreenIndex() const override;

    virtual void build() override;
    virtual void destroy() override;
    virtual void draw() override;
    virtual void update(float deltaTime, int simulationSteps) override;

    virtual void onEntry() override;
    virtual void onExit() override;

    bool isFreezed();
    bool isPaused();
    void pause();
    void resume();
    void restartLevel(bool resetCamera = true);
    void checkVictory();
    void addScore(int points);
    void addShotsFired(int shots);
    void addShotsHit(int shots);
    void addEnemyShot();
    void addGameTime(float deltaTime);
    void addHealth(float health);

    Ess2D::SpriteBatch* getSpriteBatch();
    Ess2D::Camera2D* getMainCamera();
    Ess2D::TextureCache* getTextureCache();
    EntityManager* getEntityManager();
    Level* getLevel();
    ProjectileManager* getProjectileManager();
    SceneRenderer* getSceneRenderer();
    ParticleManager* getParticleManager();
    PowerupManager* getPowerupManager();

    bool onBtnExitGameClicked(const CEGUI::EventArgs &e);
    bool onBtnFreezeGameClicked(const CEGUI::EventArgs &e);
    bool onBtnReloadLevelClicked(const CEGUI::EventArgs &e);
    
    bool onBtnPauseGameClicked(const CEGUI::EventArgs &e);
    bool onBtnResumeGameClicked(const CEGUI::EventArgs &e);
    bool onBtnNextLevelClicked(const CEGUI::EventArgs &e);
    bool onBtnRestartLevelClicked(const CEGUI::EventArgs &e);

  private:
    std::string getGameTime();
    std::string getAccuracy();
    void displayVictoryScreen();
    void displayGameOverScreen();

    void processInput(float deltaTime);
    void initUI();

    Game* _game = nullptr;

    float _targetWidth = 1280.0f;
    float _scrollSpeed = 7.0f;

    std::vector<std::string> _levelNames;
    std::string _levelName = "intro";

    bool _canPause = true;
    bool _isPaused = false;
    bool _isFreezed = false;
    bool _reloadLevel = false;

    int _score = 0;
    int _shotsFired = 0;
    int _shotsHit = 0;
    int _enemiesShotDown = 0;
    int _lives = 3;
    float _gameTime = 0;

    Ess2D::SpriteBatch _spriteBatch;
    Ess2D::Camera2D _camera;
    Ess2D::TextureCache _textureCache;
    Ess2D::FPSLimiter _fpsLimiter;
    Ess2D::FrameBufferObject* _sceneFBO;
    Ess2D::GUI _gui;

    SceneRenderer* _sceneRenderer;
    FBORenderer* _fboRenderer;
    PostProcessing* _postProcessing;
    Level* _level;
    EntityManager* _entityManager;
    ProjectileManager* _projectileManager;
    ParticleManager _particleManager;
    PowerupManager _powerupManager;

};


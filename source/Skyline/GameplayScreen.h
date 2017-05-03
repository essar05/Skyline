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

    Ess2D::SpriteBatch* getSpriteBatch();
    Ess2D::Camera2D* getMainCamera();
    Ess2D::TextureCache* getTextureCache();
    EntityManager* getEntityManager();
    Level* getLevel();
    ProjectileManager* getProjectileManager();
    SceneRenderer* getSceneRenderer();
    ParticleManager* getParticleManager();

    bool onBtnSpawnEntityClicked(const CEGUI::EventArgs &e);
    bool onBtnExitGameClicked(const CEGUI::EventArgs &e);
    bool onBtnFreezeGameClicked(const CEGUI::EventArgs &e);
    bool onBtnReloadLevelClicked(const CEGUI::EventArgs &e);

  private:
    void processInput(float deltaTime);
    void initUI();

    Game* _game = nullptr;

    float _targetWidth = 1280.0f;
    float _scrollSpeed = 7.0f;
    std::string _levelName = "intro";

    bool _canPause = true;
    bool _isPaused = false;
    bool _isFreezed = true;
    bool _reloadLevel = false;

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

};


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

  private:
    void processInput(float deltaTime);
    Game* _game = nullptr;

    float _scrollSpeed = 10.0f;

    bool _canPause = true;
    bool _isPaused = false;

    Ess2D::SpriteBatch _spriteBatch;
    Ess2D::Camera2D _camera;
    Ess2D::TextureCache _textureCache;
    Ess2D::FPSLimiter _fpsLimiter;
    Ess2D::FrameBufferObject* _sceneFBO;

    SceneRenderer* _sceneRenderer;
    FBORenderer* _fboRenderer;
    PostProcessing* _postProcessing;
    Level* _level;
    EntityManager* _entityManager;
    ProjectileManager* _projectileManager;

};


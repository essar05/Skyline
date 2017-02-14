#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <glew.h>
#include <RuntimeException.cpp>
#include <IGame.h>
#include <Window.h>
#include <Shader.h>
#include <SpriteBatch.h>
#include <Camera2D.h>
#include <TextureCache.h>
#include <InputManager.h>
#include <Timing.h>
#include "Level.h"
#include "EntityManager.h"
#include "ProjectileManager.h"

enum class GameState { RUNNING, EXIT };

class Level;
class ProjectileManager;

class Game : Ess2D::IGame {
  public:
	  void Boot();
	  static Game* GetInstance();
	  void Destroy();
	  void Render();
	  void Run();
    ~Game();

    Ess2D::SpriteBatch* getSpriteBatch();
    Ess2D::Camera2D* getMainCamera();
    Ess2D::TextureCache* getTextureCache();
    EntityManager* getEntityManager();
    Level* getLevel() { return _level; }
    ProjectileManager* getProjectileManager() { return _projectileManager; }
    float getWidth();
    float getHeight();
    float getTimeStepAccumulatorRatio() { return _timestepAccumulatorRatio; }

  protected:
    Game();

  private:
    void initShaders();
    void initSystem();
    void update(float deltaTime);
    void updateObjects(float deltaTime);
    void updateProjectiles(float deltaTime);
    void processInput(float deltaTime);

    static Game* instance;
    Ess2D::Window* _window;
    Ess2D::Shader* _baseProgram;
    
    Ess2D::SpriteBatch _spriteBatch;
    Ess2D::Camera2D _camera;
    Ess2D::TextureCache _textureCache;

    Ess2D::InputManager _inputManager;
    Ess2D::FPSLimiter _fpsLimiter;

    GameState _state;
    Level* _level;
    EntityManager* _entityManager;
    ProjectileManager* _projectileManager;

    //settings
    float _fps;
    float _maxFPS = 60.0f;
    float _timestepAccumulator = 0.0f;
    float _timestepAccumulatorRatio = 0.0f;
    bool _debugMode = false;
    bool _limitFPS = true;
    bool _vSync = 0;

    float _height = 720.0f;
    float _width = 1280.0f;
    std::string _title = "Skyline 1.0";
    float _scrollSpeed = 10.0f;
    Ess2D::WindowMode _windowMode = Ess2D::WindowMode::NORMAL;

    glm::vec2 _cameraPosition;
    glm::vec2 _previousCameraPosition;

    bool _canPause = true;
    bool _isPaused = false;

};

#endif // GAME_H

#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <glew.h>
#include <RuntimeException.cpp>
#include <IGame.h>
#include <Window.h>
#include <GLProgram.h>
#include <SpriteBatch.h>
#include <Camera2D.h>
#include <TextureCache.h>
#include <InputManager.h>
#include <Timing.h>
#include "Level.h"
#include "Player.h"
#include "EntityManager.h"
#include "ProjectileManager.h"

enum class GameState { RUNNING, EXIT };

class Game : Essengine::IGame {
  public:
	  void Boot();
	  static Game* GetInstance();
	  void Destroy();
	  void Render();
	  void Run();
    ~Game();

    Essengine::SpriteBatch* getSpriteBatch();
    Essengine::Camera2D* getMainCamera();
    Essengine::TextureCache* getTextureCache();
    EntityManager* getEntityManager();
    Level* getLevel() { return _level; }
    Player* getPlayer() { return _player; }
    ProjectileManager* getProjectileManager() { return &_projectileManager; }
    float getWidth();
    float getHeight();

  protected:
    Game();

  private:
    void initShaders();
    void initSystem();
    void calculateFPS();
    void render();
    void update(float deltaTime);
    void updateObjects(float deltaTime);
    void updatePlayer(float deltaTime);
    void updateProjectiles(float deltaTime);
    void processInput(float deltaTime);

    static Game* instance;
    Essengine::Window* _window;
    Essengine::GLProgram* _baseProgram;
    
    Essengine::SpriteBatch _spriteBatch;
    Essengine::Camera2D _camera;
    Essengine::TextureCache _textureCache;

    Essengine::InputManager _inputManager;
    Essengine::FPSLimiter _fpsLimiter;

    GameState _state;
    Level* _level;
    Player* _player;
    EntityManager _entityManager;
    ProjectileManager _projectileManager;

    //settings
    float _fps;
    float _maxFPS = 60.0f;
    bool _debugMode = false;
    bool _limitFPS = false;

    float _height = 720.0f;
    float _width = 1280.0f;
    std::string _title = "Skyline 1.0";
    float scrollSpeed = 0.2f;

    bool _canPause = true;
    bool _isPaused = false;

};

#endif // GAME_H

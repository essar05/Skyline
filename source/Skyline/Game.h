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
    float getWidth();
    float getHeight();

  protected:
    Game();

  private:
    void initShaders();
    void initSystem();
    void calculateFPS();
    void render();
    void processInput();

    static Game* instance;
    Essengine::Window* _window;
    Essengine::GLProgram* _baseProgram;
    GameState _state;
    Essengine::SpriteBatch _spriteBatch;
    Essengine::Camera2D _camera;
    Essengine::TextureCache _textureCache;

    Essengine::InputManager _inputManager;
    Essengine::FPSLimiter _fpsLimiter;

    Level _level;
    Player* _player;

    //settings
    float _fps;
    float _maxFPS = 60.0f;
    bool _debugMode = false;
    float _height = 768.0f;
    float _width = 1024.0f;
    std::string _title = "Skyline 1.0";

};

#endif // GAME_H

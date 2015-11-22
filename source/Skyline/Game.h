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

enum class GameState { RUNNING, EXIT };

class Game : Essengine::IGame {
  public:
	  void Boot();
	  static Game* GetInstance();
	  void Destroy();
	  void Render();
	  void Run();
    ~Game();

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

    float _fps;
    float _frameTime;
    float _time;

    //settings
    int _maxFps = 0;
    bool _debugMode = false;

};

#endif // GAME_H

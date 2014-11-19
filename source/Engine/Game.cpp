#include "Game.h"

using namespace Engine;

Game::Game() {}

Game::~Game() {
  delete this->renderer;
}

Game* Game::instance = NULL;

void Game::Boot() {
  this->map = new Map();
  
  this->parser = new MapParser();
  //this->parser->Parse(this->map, "Maps/aSample.emp");

  this->renderer = new Renderer(this);
}

Game* Game::GetInstance() {
  if(instance == NULL) {
    instance = new Game();
  }
  return instance;
}

void Game::Destroy() {
  delete this;
}

void Game::Run() {
  SDLW::StartTextInput();
  SDL_Event theEvent;

  this->doRun(&theEvent);

  SDLW::StopTextInput();
}

void Game::doRun(SDL_Event* anEvent) {
  while(SDLW::PollEvent(anEvent) != 0) {
    if(anEvent->type == SDL_QUIT) {
      return;
    }
    else if(anEvent->type == SDL_TEXTINPUT) {
      /*
      int x = 0, y = 0;
      SDL_GetMouseState( &x, &y );
      handleKeys( e.text.text[ 0 ], x, y );
      */
    }
  }

  this->renderer->Render();
  this->renderer->SwapWindow();

  this->doRun(anEvent);
}

void Game::generateTempMap() {
  this->map->SetLayersCount(1);
  this->map->SetWidth(100);
  this->map->SetHeight(100);
  this->map->Tiles = new int**[this->map->GetLayersCount()];
  this->map->Walkable = new bool*[this->map->GetWidth()];
  this->map->Passable = new bool*[this->map->GetWidth()];
  for(int i = 0; i < this->map->GetWidth(); i++) {
    this->map->Walkable[i] = new bool[this->map->GetHeight()];
    this->map->Passable[i] = new bool[this->map->GetHeight()];
  }

  for(int i = 0; i < this->map->GetLayersCount(); i++) {
    this->map->Tiles[i] = new int*[this->map->GetWidth()];

    for(int j = 0; j < this->map->GetWidth(); j++) {
      this->map->Tiles[i][j] = new int[this->map->GetHeight()];

      for(int k = 0; k < this->map->GetHeight(); k++) {
        this->map->Tiles[i][j][k] = 1;
      }
    }
  }
}

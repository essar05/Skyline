#include "Game.h"

using namespace Engine;

Game::Game() {}

Game::~Game() {
  delete this->renderer;
}

Game* Game::instance = NULL;

void Game::Boot() {
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

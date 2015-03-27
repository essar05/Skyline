#include "Game.h"
#include "Tile.h"

using namespace Engine;

Game::Game() {}

Game::~Game() {
	delete this->renderer;
}

Game* Game::instance = NULL;

void Game::Boot() {
	this->map = new Map(128, 64); //to be edited

	this->parser = new MapParser();
	this->generateTempMap();
	//this->parser->Parse(this->map, "Maps/aSample.emp");

	this->renderer = new Renderer(this);

	this->map->ComputeTilePositions(this->viewportOffsetX, this->viewportOffsetY, this->width, this->height);
}

Game* Game::GetInstance() {
	if (instance == NULL) {
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

	while (this->doRun(&theEvent));

	SDLW::StopTextInput();
}

bool Game::doRun(SDL_Event* anEvent) {
	while (SDLW::PollEvent(anEvent) != 0) {
		if (anEvent->type == SDL_QUIT) {
			return false;
		}
		else if (anEvent->type == SDL_MOUSEBUTTONDOWN) {
			if (anEvent->button.button == SDL_BUTTON_LEFT) {
				this->isMouseLeftDown = true;
			}
		}
		else if (anEvent->type == SDL_MOUSEBUTTONUP) {
			if (anEvent->button.button == SDL_BUTTON_LEFT) {
				this->isMouseLeftDown = false;
			}
		}
		else if (anEvent->type == SDL_MOUSEMOTION && this->isMouseLeftDown) {
			if (this->viewportOffsetX - anEvent->motion.xrel > 0 && (this->map->GetOffsetJ() + 1 <= this->map->GetWidth() - this->map->GetOffsetWidth() || this->viewportOffsetX > this->viewportOffsetX - anEvent->motion.xrel)) {
				this->viewportOffsetX -= anEvent->motion.xrel;
			}
			if (this->viewportOffsetY + anEvent->motion.yrel < 0 && (this->map->GetOffsetI() + 1 <= this->map->GetHeight() - this->map->GetOffsetHeight() || this->viewportOffsetY < this->viewportOffsetY + anEvent->motion.yrel)) {
				this->viewportOffsetY += anEvent->motion.yrel;
			}
			printf(" (%d, %d)\n", this->viewportOffsetX, this->viewportOffsetY);
			this->map->ComputeTilePositions(this->viewportOffsetX, this->viewportOffsetY, this->width, this->height);
		}
	}

	this->renderer->Render();

	this->renderer->SwapWindow();

	return true;
}

Map* Game::GetMap() {
	return this->map;
}

int Game::GetHeight() {
	return this->height;
}

int Game::GetWidth() {
	return this->width;
}

int Game::GetViewportOffsetX() {
	return this->viewportOffsetX;
}

int Game::GetViewportOffsetY() {
	return this->viewportOffsetY;
}

void Game::SetHeight(int aHeight) {
	this->height = aHeight;
}

void Game::SetWidth(int aWidth) {
	this->width = aWidth;
}


void Game::generateTempMap() {
	this->map->SetLayersCount(1);
	this->map->SetWidth(50);
	this->map->SetHeight(50);
	this->map->Tiles = new Tile***[this->map->GetLayersCount()];
	this->map->Walkable = new bool*[this->map->GetWidth()];
	this->map->Passable = new bool*[this->map->GetWidth()];
	for (int i = 0; i < this->map->GetWidth(); i++) {
		this->map->Walkable[i] = new bool[this->map->GetHeight()];
		this->map->Passable[i] = new bool[this->map->GetHeight()];
	}

	for (int i = 0; i < this->map->GetLayersCount(); i++) {
		this->map->Tiles[i] = new Tile**[this->map->GetWidth()];

		for (int j = 0; j < this->map->GetWidth(); j++) {
			this->map->Tiles[i][j] = new Tile*[this->map->GetHeight()];

			for (int k = 0; k < this->map->GetHeight(); k++) {
				this->map->Tiles[i][j][k] = new Tile(1, 0, 0);
			}
		}
	}
}
#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <glew.h>
#include "RuntimeException.cpp"
#include "SDLW.h"
#include "GLW.h"
#include "Shader.h"
#include "Renderer.h"
#include "Map.h"
#include "MapParser.h"
#include "Timer.h"

namespace Engine {
	class Renderer;
	class Game {
	public:
		void Boot();
		static Game* GetInstance();
		void Destroy();
		void Render();
		void Run();
		Map* GetMap();
		int GetHeight();
		int GetWidth();
		int GetViewportOffsetX();
		int GetViewportOffsetY();
		void SetHeight(int aHeight);
		void SetWidth(int aWidth);

	private:
		Game();
		~Game();
		bool doRun(SDL_Event* anEvent);
		void generateTempMap();

		static Game* instance;
		Renderer* renderer;
		Map* map;
		MapParser* parser;
		int width;
		int height;
		int viewportOffsetX = 0;
		int viewportOffsetY = 0;
		int countedFrames = 0;
		LTimer* fpsTimer;
		int isMouseLeftDown = false;
		int mouseX = 0;
		int mouseY = 0;

	};

};

#endif // GAME_H

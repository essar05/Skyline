#ifndef RENDERER_H
#define RENDERER_H

#include "SDLW.h"
#include "GLW.h"
#include "Texture.h"
#include "Model.h"
#include "Grass.h"
#include "Game.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include <map>
#include <string>

using namespace std;
using namespace Engine;
using namespace Terrain;

typedef map<int, Model*> modelMap;
typedef map<int, GLuint> buffersMap;

namespace Engine {
  class Game;
  class Renderer {
    public:
      Renderer(Game* aGameInstance);
      virtual ~Renderer();
      void Render();
      void SwapWindow();
	  GLuint GetWidth();
	  GLuint GetHeight();
    protected:
    private:
      Game* game;
      Texture* textureAtlas;
      modelMap* models;
	  buffersMap* buffers;
      GLuint* UBO;
      GLuint* VBO;
      bool renderQuad = true;
      GLint vertexPos2DLocation = -1;
      SDL_GLContext context;
      GLuint programId = 0;
      SDL_Window* window = NULL;
      GLuint textureId;
      GLuint matrixId;
      glm::mat4 MVP;
      GLuint screenWidth;
      GLuint screenHeight;
  };
};

#endif // RENDERER_H

#include <SDL.h>
#include <glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>
#include <stdio.h>
#include <string>

namespace Pixeltwo {

  class CGame {

    public:
      CGame();
      ~CGame();
      void boot();
      bool run();

    private:
      const int SCREEN_WIDTH = 640;
      const int SCREEN_HEIGHT = 480;
      SDL_Window* gWindow = NULL;
      SDL_GLContext gContext;
      bool gRenderQuad = true;
      GLuint gProgramID = 0;
      GLint gVertexPos2DLocation = -1;
      GLuint gVBO = 0;
      GLuint gIBO = 0;

  };

};



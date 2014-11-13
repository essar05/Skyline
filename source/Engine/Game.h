namespace Engine {

  class CGame {

    public:
      void Boot();
      static CGame* GetInstance();
      void Destroy();
      void Render();
      void Run();

    private:
      CGame();
      ~CGame();
      void doRun(SDL_Event* e);

      SDL_GLContext context;
      static CGame* instance;
      GLuint IBO = 0;
      GLuint programId = 0;
      bool renderQuad = true;
      GLint vertexPos2DLocation = -1;
      GLuint VBO = 0;
      SDL_Window* window = NULL;

  };

};



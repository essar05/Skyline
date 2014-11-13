#include "Game.h"

using namespace Engine;

CGame::CGame() {}

CGame::~CGame() {
	GLW::DeleteProgram(this->programId);

	SDLW::DestroyWindow_(this->window);

	SDLW::Quit();
}

CGame* CGame::instance = NULL;

void CGame::Boot() {
  SDLW::Init(SDL_INIT_VIDEO);

  /*SDL_DisplayMode* current = new SDL_DisplayMode;

  int should_be_zero = SDL_GetCurrentDisplayMode(0, current);

  if(should_be_zero != 0)
    // In case of error...
    SDL_Log("Could not get display mode for video display #%d: %s", 0, SDL_GetError());

  else
    // On success, print the current display mode.
    SDL_Log("Display #%d: current display mode is %dx%dpx @ %dhz. \n", 0, current->w, current->h, current->refresh_rate);
  */

  SDLW::SetGLAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDLW::SetGLAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
  SDLW::SetGLAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  this->window = SDLW::CreateWindow_("SDL Project", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP);
  this->context = SDLW::GLCreateContext(this->window);

  SDLW::GLEWInit();
  SDLW::GLSetSwapInterval(1);

	this->programId = GLW::CreateProgram();

	GLuint vertexShader = GLW::CreateShader(GL_VERTEX_SHADER);
	GLW::ShaderSource(vertexShader, 1, GLW::GetVertexShaderSource(), NULL);
	GLW::CompileShader(vertexShader);
  GLW::AttachShader(this->programId, vertexShader);

  GLuint fragmentShader = GLW::CreateShader(GL_FRAGMENT_SHADER);
  GLW::ShaderSource(fragmentShader, 1, GLW::GetFragmentShaderSource(), NULL);
  GLW::CompileShader(fragmentShader);
  GLW::AttachShader(this->programId, fragmentShader);

  GLW::LinkProgram(this->programId);

  this->vertexPos2DLocation = glGetAttribLocation(this->programId, "LVertexPos2D");

  GLW::ClearColor(0.f, 0.f, 0.f, 1.f);

  GLfloat vertexData[] =
  {
    -0.5f, -0.5f,
     0.5f, -0.5f,
     0.5f,  0.5f,
    -0.5f,  0.5f
  };
  GLuint indexData[] = {0, 1, 2, 3};

  //Create VBO
  GLW::GenBuffers(1, &this->VBO);
  GLW::BindBuffer(GL_ARRAY_BUFFER, this->VBO);
  GLW::BufferData(GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);

  //Create IBO
  GLW::GenBuffers(1, &this->IBO);
  GLW::BindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->IBO);
  GLW::BufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), indexData, GL_STATIC_DRAW);
}

CGame* CGame::GetInstance() {
  if(instance == NULL) {
    instance = new CGame();
  }
  return instance;
}

void CGame::Destroy() {
  delete this;
}

void CGame::Render() {
	GLW::Clear(GL_COLOR_BUFFER_BIT);

	if(!this->renderQuad) {
	  return;
	}

  GLW::UseProgram(this->programId);

  GLW::EnableVertexAttribArray(this->vertexPos2DLocation);

  GLW::BindBuffer(GL_ARRAY_BUFFER, this->VBO);
  GLW::VertexAttribPointer(this->vertexPos2DLocation, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);

  GLW::BindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->IBO);
  GLW::DrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL);

  GLW::DisableVertexAttribArray(this->vertexPos2DLocation);

  GLW::UseProgram(0);
}

void CGame::Run() {
  SDLW::StartTextInput();
  SDL_Event theEvent;

  this->doRun(&theEvent);

  SDLW::StopTextInput();
}

void CGame::doRun(SDL_Event* anEvent) {
  while(SDLW::PollEvent(anEvent) != 0) {
    if(anEvent->type == SDL_QUIT) {
      return;
    }
    else if(anEvent->type == SDL_TEXTINPUT)
    {
      /*
      int x = 0, y = 0;
      SDL_GetMouseState( &x, &y );
      handleKeys( e.text.text[ 0 ], x, y );
      */
    }
  }

  this->Render();

  SDLW::GLSwapWindow(this->window);

  this->doRun(anEvent);
}

#include "MainMenuScreen.h"

MainMenuScreen::MainMenuScreen() {
  _game = Game::GetInstance();
  _screenIndex = SCREEN_INDEX_MAINMENU;
}

MainMenuScreen::~MainMenuScreen() {
}

int MainMenuScreen::getNextScreenIndex() const {
  return SCREEN_INDEX_GAMEPLAY;
}

int MainMenuScreen::getPreviousScreenIndex() const {
  return SCREEN_INDEX_NO_SCREEN;
}

void MainMenuScreen::build() {
  _gui.init("GUI");
  _gui.loadScheme("TaharezLook.scheme");
  _gui.loadScheme("AlfiskoSkin.scheme");
  _gui.loadScheme("Generic.scheme");
  _gui.loadScheme("Skyline.scheme");
  _gui.setFont("DejaVuSans-10");

  _gui.setMouseCursor("TaharezLook/MouseArrow");
  _gui.showMouseCursor();

  CEGUI::Window* menuRoot = _gui.loadLayout("SkylineMainMenu.layout");
  menuRoot->getChild("StartGameContainer")->subscribeEvent(CEGUI::DefaultWindow::EventMouseClick, CEGUI::Event::Subscriber(&MainMenuScreen::onBtnStartGameClicked, this));
  menuRoot->getChild("CreditsContainer")->subscribeEvent(CEGUI::DefaultWindow::EventMouseClick, CEGUI::Event::Subscriber(&MainMenuScreen::onBtnCreditsClicked, this));
  menuRoot->getChild("ExitGameContainer")->subscribeEvent(CEGUI::DefaultWindow::EventMouseClick, CEGUI::Event::Subscriber(&MainMenuScreen::onBtnExitClicked, this));
}

void MainMenuScreen::destroy() {

}

void MainMenuScreen::onEntry() {

}

void MainMenuScreen::onExit() {

}

void MainMenuScreen::update(float deltaTime, int simulationSteps) {
  for(int i = 0; i < simulationSteps; i++) {
    processInput(deltaTime);
  }
}

void MainMenuScreen::processInput(float deltaTime) {
  SDL_Event event;

  while(SDL_PollEvent(&event) != 0) {
    _gui.onSDLEvent(event);
    _game->onSDLEvent(event);
  }
}

void MainMenuScreen::draw() {
  glClearDepth(1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  _gui.draw();
}

bool MainMenuScreen::onBtnStartGameClicked(const CEGUI::EventArgs &e) {
  _currentState = Ess2D::ScreenState::CHANGE_NEXT;
  return true;
}

bool MainMenuScreen::onBtnCreditsClicked(const CEGUI::EventArgs &e) {
  std::cout << "CREDITS CLICKED" << std::endl;
  return true;
}

bool MainMenuScreen::onBtnExitClicked(const CEGUI::EventArgs &e) {
  _currentState = Ess2D::ScreenState::EXIT_APPLICATION;
  return true;
}
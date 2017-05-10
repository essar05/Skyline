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
  _gui.loadScheme("Generic.scheme");
  _gui.loadScheme("Skyline.scheme");
  _gui.setFont("Neuropol-11");

  _gui.setMouseCursor("TaharezLook/MouseArrow");
  _gui.showMouseCursor();

  CEGUI::Window* menuRoot = _gui.loadLayout("SkylineMainMenu.layout");
  menuRoot->getChild("StartGameContainer")->subscribeEvent(CEGUI::DefaultWindow::EventMouseClick, CEGUI::Event::Subscriber(&MainMenuScreen::onBtnStartGameClicked, this));
  menuRoot->getChild("CreditsContainer")->subscribeEvent(CEGUI::DefaultWindow::EventMouseClick, CEGUI::Event::Subscriber(&MainMenuScreen::onBtnCreditsClicked, this));
  menuRoot->getChild("ExitGameContainer")->subscribeEvent(CEGUI::DefaultWindow::EventMouseClick, CEGUI::Event::Subscriber(&MainMenuScreen::onBtnExitClicked, this));

  menuRoot->getChild("StartGameContainer")->subscribeEvent(CEGUI::DefaultWindow::EventMouseEntersArea, CEGUI::Event::Subscriber(&MainMenuScreen::onBtnStartGameMouseOver, this));
  menuRoot->getChild("StartGameContainer")->subscribeEvent(CEGUI::DefaultWindow::EventMouseLeavesArea, CEGUI::Event::Subscriber(&MainMenuScreen::onBtnStartGameMouseOut, this));
  menuRoot->getChild("CreditsContainer")->subscribeEvent(CEGUI::DefaultWindow::EventMouseEntersArea, CEGUI::Event::Subscriber(&MainMenuScreen::onBtnCreditsMouseOver, this));
  menuRoot->getChild("CreditsContainer")->subscribeEvent(CEGUI::DefaultWindow::EventMouseLeavesArea, CEGUI::Event::Subscriber(&MainMenuScreen::onBtnCreditsMouseOut, this));
  menuRoot->getChild("ExitGameContainer")->subscribeEvent(CEGUI::DefaultWindow::EventMouseEntersArea, CEGUI::Event::Subscriber(&MainMenuScreen::onBtnExitGameMouseOver, this));
  menuRoot->getChild("ExitGameContainer")->subscribeEvent(CEGUI::DefaultWindow::EventMouseLeavesArea, CEGUI::Event::Subscriber(&MainMenuScreen::onBtnExitGameMouseOut, this));
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

bool MainMenuScreen::onBtnStartGameMouseOver(const CEGUI::EventArgs &e) {
  toggleMainMenuButtonHover("MainMenuRoot/StartGameContainer/Label", true);
  return true;
}

bool MainMenuScreen::onBtnStartGameMouseOut(const CEGUI::EventArgs &e) {
  toggleMainMenuButtonHover("MainMenuRoot/StartGameContainer/Label", false);
  return true;
}

bool MainMenuScreen::onBtnCreditsMouseOver(const CEGUI::EventArgs &e) {
  toggleMainMenuButtonHover("MainMenuRoot/CreditsContainer/Label", true);
  return true;
}

bool MainMenuScreen::onBtnCreditsMouseOut(const CEGUI::EventArgs &e) {
  toggleMainMenuButtonHover("MainMenuRoot/CreditsContainer/Label", false);
  return true;
}

bool MainMenuScreen::onBtnExitGameMouseOver(const CEGUI::EventArgs &e) {
  toggleMainMenuButtonHover("MainMenuRoot/ExitGameContainer/Label", true);
  return true;
}

bool MainMenuScreen::onBtnExitGameMouseOut(const CEGUI::EventArgs &e) {
  toggleMainMenuButtonHover("MainMenuRoot/ExitGameContainer/Label", false);
  return true;
}

void MainMenuScreen::toggleMainMenuButtonHover(const std::string& elementName, bool isHovering) {
  _gui.getRootWindow()->getChild(elementName)->setProperty("NormalTextColour", isHovering ? _buttonHoverColor : _buttonNormalColor);
}
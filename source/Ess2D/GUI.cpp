#include "GUI.h"

namespace Ess2D {

  GUI::GUI() {}

  GUI::~GUI() {}

  void GUI::init(const std::string& resourcesPath) {
    if(_renderer == nullptr) {
      _renderer = &CEGUI::OpenGL3Renderer::bootstrapSystem();

      CEGUI::DefaultResourceProvider* resourceProvider = static_cast<CEGUI::DefaultResourceProvider*> (CEGUI::System::getSingleton().getResourceProvider());
      resourceProvider->setResourceGroupDirectory("imagesets", resourcesPath + "/imagesets/");
      resourceProvider->setResourceGroupDirectory("schemes", resourcesPath + "/schemes/");
      resourceProvider->setResourceGroupDirectory("fonts", resourcesPath + "/fonts/");
      resourceProvider->setResourceGroupDirectory("layouts", resourcesPath + "/layouts/");
      resourceProvider->setResourceGroupDirectory("looknfeel", resourcesPath + "/looknfeel/");
      resourceProvider->setResourceGroupDirectory("lua_scripts", resourcesPath + "/lua_scripts/");
      //resourceProvider->setResourceGroupDirectory("xml_schemes", resourcesPath + "/xml_schemes/");
      resourceProvider->setResourceGroupDirectory("animations", resourcesPath + "/animations/");

      CEGUI::ImageManager::setImagesetDefaultResourceGroup("imagesets");
      CEGUI::Scheme::setDefaultResourceGroup("schemes");
      CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeel");
      CEGUI::Font::setDefaultResourceGroup("fonts");
      CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");
      CEGUI::WindowManager::setDefaultResourceGroup("layouts");
      CEGUI::AnimationManager::setDefaultResourceGroup("animations");
    }

    _context = &CEGUI::System::getSingleton().createGUIContext(_renderer->getDefaultRenderTarget());
    _root = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "root");
    _context->setRootWindow(_root);
  }

  void GUI::destroy() {
    CEGUI::System::getSingleton().destroyGUIContext(*_context);
  }

  void GUI::draw() {
    _renderer->beginRendering();
    _context->draw();
    _renderer->endRendering();
    glDisable(GL_SCISSOR_TEST);
  }

  void GUI::loadScheme(const std::string& schemeFile) {
    CEGUI::SchemeManager::getSingleton().createFromFile(schemeFile);
  }

  void GUI::setFont(const std::string& fontFile) {
    CEGUI::FontManager::getSingleton().createFromFile(fontFile + ".font");
    _context->setDefaultFont(fontFile);
  }

  CEGUI::Window* GUI::createWidget(const std::string& type, const glm::vec4& destRectPercentage, const glm::vec4& destRectPixel, const std::string& name/* = ""*/) {
    CEGUI::Window* window = CEGUI::WindowManager::getSingleton().createWindow(type, name);
    setWidgetDestRect(window, destRectPercentage, destRectPixel);
    _root->addChild(window);
    return window;
  }

  void GUI::setWidgetDestRect(CEGUI::Window* widget, const glm::vec4& destRectPercentage, const glm::vec4& destRectPixel) {
    widget->setPosition(CEGUI::UVector2(CEGUI::UDim(destRectPercentage.x, destRectPixel.x), CEGUI::UDim(destRectPercentage.y, destRectPixel.y)));
    widget->setSize(CEGUI::USize(CEGUI::UDim(destRectPercentage.z, destRectPixel.z), CEGUI::UDim(destRectPercentage.w, destRectPixel.w)));
  }

  CEGUI::OpenGL3Renderer* GUI::_renderer = nullptr;

}
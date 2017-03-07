#pragma once
#include <CEGUI\CEGUI.h>
#include <CEGUI\RendererModules\OpenGL\GL3Renderer.h>
#include <glm\glm.hpp>
#include <SDL_events.h>

namespace Ess2D {
  class GUI {
    public:
      GUI();
      ~GUI();

      void init(const std::string& resourcesPath);
      void destroy();
      void draw();
      void update();
      
      void loadScheme(const std::string& schemeFile);
      void setFont(const std::string& fontFile);
      CEGUI::Window* createWidget(const std::string& type, const glm::vec4& destRectPercentage, const glm::vec4& destRectPixel, const std::string& name = "" );

      void setMouseCursor(const std::string& imageFile);
      void showMouseCursor();
      void hideMouseCursor();

      void onSDLEvent(SDL_Event& event);

      static void setWidgetDestRect(CEGUI::Window* widget, const glm::vec4& destRectPercentage, const glm::vec4& destRectPixel);

      static CEGUI::OpenGL3Renderer* getRenderer() { return _renderer; }
      const CEGUI::GUIContext* getContext() { return _context; }

    private:
      static CEGUI::OpenGL3Renderer* _renderer;
      CEGUI::GUIContext* _context = nullptr;
      CEGUI::Window* _root = nullptr;

      unsigned int _lastTimeStamp = 0;

  };
}
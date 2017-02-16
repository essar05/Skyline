#pragma once

namespace Ess2D {

  class IGame
  {
    public:
      virtual void Boot() = 0;
      virtual void Destroy() = 0;
      virtual void Run() = 0;
      virtual ~IGame() {}

  };

}
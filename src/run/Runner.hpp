#pragma once
#include <vector>

#include "GameDataStructs.hpp"
#include "SDL3/SDL_render.h"
#include "sol/sol.hpp"

namespace Vania {

class Runner {
 public:
  const int DISPLAY_WIDTH = 700;
  const int DISPLAY_HEIGHT = 700;

  SDL_Texture* displayTexture;

 private:
  SDL_Renderer* renderer;
  sol::state lua;
  GameData gameDataCopy;
  const std::filesystem::path& root;

 public:
  Runner(const GameData& gameData, SDL_Renderer* renderer);
  ~Runner();
  void reset(const GameData& gameData);
  void runAllScriptsSetups();
  void update();
  void render();
};

};  // namespace Vania

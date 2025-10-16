#pragma once
#include <filesystem>
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
  // std::vector<RuntimeEntity> entities;
  std::filesystem::path root;

 public:
  Runner(const nlohmann::json& gameData, SDL_Renderer* renderer);
  ~Runner();
  void reset(const nlohmann::json& gameData);
  void runAllScriptsSetups();
  void update();
  void render();
};

};  // namespace Vania

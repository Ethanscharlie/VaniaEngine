#pragma once
#include <vector>

#include "GameDataStructs.hpp"
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_timer.h"
#include "panels/EditorContext.hpp"
#include "render/RendererForSDL.hpp"
#include "sol/sol.hpp"

namespace Vania {

class Runner {
 public:
  const int DISPLAY_WIDTH = 1920;
  const int DISPLAY_HEIGHT = 1080;

  SDL_Texture* displayTexture;
  RendererForSDL vaniaRenderer;

 private:
  sol::state lua;
  EditorContext& context;
  GameData gameDataCopy;
  const std::filesystem::path& root;
  Uint32 lastTime = SDL_GetTicks();
  SDL_FPoint mousePosition = {0, 0};

 public:
  Runner(EditorContext& context);
  ~Runner();
  void reset(const GameData& gameData);
  void runAllScriptsSetups();
  void updateMousePos(const SDL_FPoint& mousePos);
  void update();
  void render();

  // Util ig
  EntityDef* getDefFromName(const std::string& name);
};

};  // namespace Vania

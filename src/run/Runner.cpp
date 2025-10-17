#include "Runner.hpp"

#include <print>

#include "GameDataStructs.hpp"
#include "SDL3/SDL_render.h"
#include "run/AssetManager.hpp"
#include "sol/forward.hpp"

namespace Vania {

Runner::Runner(const nlohmann::json& gameData, SDL_Renderer* renderer)
    : root(gameData["editor"]["rootPath"]), renderer(renderer) {
  displayTexture =
      SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, DISPLAY_WIDTH, DISPLAY_HEIGHT);

  lua.open_libraries(sol::lib::base, sol::lib::package);

  // lua.set_function("summon", [this, gameData](const std::string& name, float x, float y) {
  //   for (const EntityDef& def : gameData.entityDefs) {
  //     if (def.name == name) {
  //       entities.push_back({def, x, y});
  //       return;
  //     }
  //   }
  // });

  reset(gameData);
}

Runner::~Runner() { SDL_DestroyTexture(displayTexture); }

void Runner::reset(const nlohmann::json& gameData) {
  gameDataCopy = gameData;
  render();
}

void Runner::runAllScriptsSetups() {
  for (auto& entity : gameDataCopy["entities"]) {
    const std::string id = entity["defId"];
    const auto& defs = gameDataCopy["defs"][id];
    const std::string& script = defs["script"];
    if (script == "") continue;
    sol::table funcs = lua.script_file(root / script);
    funcs["setup"](&entity);
  }
}

void Runner::update() {
  for (auto& entity : gameDataCopy["entities"]) {
    const std::string id = entity["defId"];
    const auto& defs = gameDataCopy["defs"][id];
    const std::string& script = defs["script"];
    if (script == "") continue;
    sol::table funcs = lua.script_file(root / script);
    funcs["update"](&entity);

    const bool* keystates = SDL_GetKeyboardState(NULL);
    if (keystates[SDL_SCANCODE_SPACE]) {
      funcs["keyHeldSpace"](&entity);
    }
    if (keystates[SDL_SCANCODE_RIGHT] || keystates[SDL_SCANCODE_D]) {
      funcs["keyHeldRight"](&entity);
    }
    if (keystates[SDL_SCANCODE_LEFT] || keystates[SDL_SCANCODE_A]) {
      funcs["keyHeldLeft"](&entity);
    }
    if (keystates[SDL_SCANCODE_UP] || keystates[SDL_SCANCODE_W]) {
      funcs["keyHeldUp"](&entity);
    }
    if (keystates[SDL_SCANCODE_DOWN] || keystates[SDL_SCANCODE_S]) {
      funcs["keyHeldDown"](&entity);
    }
  }

  render();
}

void Runner::render() {
  SDL_SetRenderTarget(renderer, displayTexture);

  SDL_SetRenderDrawColor(renderer, 0, 0, 10, 255);
  SDL_RenderClear(renderer);

  for (const auto& entity : gameDataCopy["entities"]) {
    const std::string id = entity["defId"];
    const auto& defs = gameDataCopy["defs"][id];
    SDL_SetRenderDrawColor(renderer, defs["r"], defs["g"], defs["b"], defs["a"]);
    SDL_FRect rect = {entity["x"], entity["y"], (float)defs["width"], (float)defs["height"]};
    SDL_RenderFillRect(renderer, &rect);

    SDL_Texture* texture = AssetManager::getInstance().get(renderer, root / "img.png");
    SDL_RenderTexture(renderer, texture, nullptr, &rect);
  }

  SDL_SetRenderTarget(renderer, nullptr);
}

}  // namespace Vania

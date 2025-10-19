#include "Runner.hpp"

#include "GameDataStructs.hpp"
#include "SDL3/SDL_render.h"
#include "render.hpp"
#include "run/utils.hpp"
#include "sol/forward.hpp"

namespace Vania {

Runner::Runner(const GameData& gameData, SDL_Renderer* renderer)
    : root(gameData.editorData.rootPath), renderer(renderer) {
  displayTexture =
      SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, DISPLAY_WIDTH, DISPLAY_HEIGHT);

  lua.open_libraries(sol::lib::base, sol::lib::package);

  lua.set_function("summon", [this, gameData](const std::string& name, float x, float y) {
    for (auto& [id, def] : gameDataCopy.entityDefs) {
      if (def.name == name) {
        gameDataCopy.worldData.entities.push_back({id, x, y});
        return;
      }
    }
  });

  EntityDef::exposeToLua(lua);
  Entity::exposeToLua(lua);
  WorldData::exposeToLua(lua);
  utils::exposeAll(lua);

  reset(gameData);
}

Runner::~Runner() { SDL_DestroyTexture(displayTexture); }

void Runner::reset(const GameData& gameData) {
  gameDataCopy = gameData;

  for (auto& entity : gameDataCopy.worldData.entities) {
    const EntityDef& def = gameData.entityDefs.at(entity.defID);
    entity.entityDefOverride = def;
  }

  render();
}

void Runner::runAllScriptsSetups() {
  for (auto& entity : gameDataCopy.worldData.entities) {
    const std::string& script = entity.entityDefOverride.script;
    if (script == "") continue;
    sol::table funcs = lua.script_file(root / script);
    funcs["setup"](&entity);
  }
}

void Runner::update() {
  for (auto& entity : gameDataCopy.worldData.entities) {
    const std::string& script = entity.entityDefOverride.script;
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

  for (auto& entity : gameDataCopy.worldData.entities) {
    Render::render(renderer, entity, entity.entityDefOverride, gameDataCopy.editorData.rootPath);
  }

  SDL_SetRenderTarget(renderer, nullptr);
}

}  // namespace Vania

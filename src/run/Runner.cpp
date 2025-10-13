#include "Runner.hpp"
#include "GameDataStructs.hpp"
#include "SDL3/SDL_render.h"
#include "run/utils.hpp"
#include "sol/forward.hpp"
#include <print>

namespace Vania {

Runner::Runner(const GameData &gameData, SDL_Renderer *renderer)
    : root(gameData.editorData.rootPath), renderer(renderer) {

  displayTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                                     SDL_TEXTUREACCESS_TARGET, DISPLAY_WIDTH,
                                     DISPLAY_HEIGHT);

  lua.open_libraries(sol::lib::base, sol::lib::package);

  lua.new_usertype<EntityDef>("EntityDef", "width", &EntityDef::width, "height",
                              &EntityDef::height);

  lua.new_usertype<RuntimeEntity>("Entity", "x", &RuntimeEntity::x, "y",
                                  &RuntimeEntity::y, "def",
                                  &RuntimeEntity::entityDef);

  lua.set_function("summon",
                   [this, gameData](const std::string &name, float x, float y) {
                     for (const EntityDef &def : gameData.entityDefs) {
                       if (def.name == name) {
                         entities.push_back({def, x, y});
                         return;
                       }
                     }
                   });

  utils::exposeAll(lua);

  reset(gameData);
}

Runner::~Runner() { SDL_DestroyTexture(displayTexture); }

void Runner::reset(const GameData &gameData) {
  entities.clear();
  for (const Entity &entity : gameData.worldData.entities) {
    entities.push_back({*entity.entityDef, entity.x, entity.y});
  }

  render();
}

void Runner::runAllScriptsSetups() {
  for (auto &entity : entities) {
    const std::string &script = entity.entityDef.script;
    if (script == "")
      continue;
    lua.script_file(root / script);
    lua["setup"](&entity);
  }
}

void Runner::update() {
  for (auto &entity : entities) {
    const std::string &script = entity.entityDef.script;
    if (script == "")
      continue;
    lua.script_file(root / script);
    lua["update"](&entity);

    const bool *keystates = SDL_GetKeyboardState(NULL);
    if (keystates[SDL_SCANCODE_SPACE]) {
      lua["keyHeldSpace"](&entity);
    }
    if (keystates[SDL_SCANCODE_RIGHT] || keystates[SDL_SCANCODE_D]) {
      lua["keyHeldRight"](&entity);
    }
    if (keystates[SDL_SCANCODE_LEFT] || keystates[SDL_SCANCODE_A]) {
      lua["keyHeldLeft"](&entity);
    }
    if (keystates[SDL_SCANCODE_UP] || keystates[SDL_SCANCODE_W]) {
      lua["keyHeldUp"](&entity);
    }
    if (keystates[SDL_SCANCODE_DOWN] || keystates[SDL_SCANCODE_S]) {
      lua["keyHeldDown"](&entity);
    }
  }

  render();
}

void Runner::render() {
  SDL_SetRenderTarget(renderer, displayTexture);

  SDL_SetRenderDrawColor(renderer, 0, 0, 10, 255);
  SDL_RenderClear(renderer);

  for (auto &entity : entities) {
    SDL_SetRenderDrawColor(renderer, 136, 42, 230, 255);
    SDL_FRect rect = {(float)entity.x, (float)entity.y,
                      (float)entity.entityDef.width,
                      (float)entity.entityDef.height};
    SDL_RenderFillRect(renderer, &rect);
  }

  SDL_SetRenderTarget(renderer, nullptr);
}

} // namespace Vania

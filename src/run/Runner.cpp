#include "Runner.hpp"
#include "GameDataStructs.hpp"
#include "SDL3/SDL_oldnames.h"
#include "SDL3/SDL_render.h"
#include "sol/forward.hpp"
#include <print>

namespace Vania {

Runner::Runner(const GameData &gameData, SDL_Renderer *renderer)
    : gameData(gameData), renderer(renderer) {
  reset();

  displayTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                                     SDL_TEXTUREACCESS_TARGET, DISPLAY_WIDTH,
                                     DISPLAY_HEIGHT);

  lua.open_libraries(sol::lib::base, sol::lib::package);

  lua.new_usertype<Entity>("Entity", "x", &Entity::x, "y", &Entity::y);

  lua.script("function run(entity)\n"
             "entity.x = entity.x + 100\n"
             "end");

  sol::function func = lua["run"];
  func(&instanceOfGameData.worldData.entities[0]);
}

Runner::~Runner() { SDL_DestroyTexture(displayTexture); }

void Runner::reset() { instanceOfGameData = gameData; }

void Runner::update() {
  SDL_SetRenderTarget(renderer, displayTexture);

  SDL_SetRenderDrawColor(renderer, 0, 0, 10, 255);
  SDL_RenderClear(renderer);

  for (Entity &entity : instanceOfGameData.worldData.entities) {
    SDL_SetRenderDrawColor(renderer, 136, 42, 230, 255);
    SDL_FRect rect = {(float)entity.x, (float)entity.y,
                      (float)entity.entityDef->width,
                      (float)entity.entityDef->height};
    SDL_RenderFillRect(renderer, &rect);
  }

  SDL_SetRenderTarget(renderer, nullptr);
}

} // namespace Vania

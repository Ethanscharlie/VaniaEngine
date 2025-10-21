#include "Runner.hpp"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>

#include "GameDataStructs.hpp"
#include "SDL3/SDL_render.h"
#include "run/AssetManager.hpp"
#include "run/utils.hpp"
#include "sol/forward.hpp"

namespace Vania {

Runner::Runner(const GameData& gameData, SDL_Renderer* renderer)
    : root(gameData.editorData.rootPath), renderer(renderer) {
  displayTexture =
      SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, DISPLAY_WIDTH, DISPLAY_HEIGHT);

  lua.open_libraries(sol::lib::base, sol::lib::package);

  lua.set_function("summon", [this, gameData](const std::string& name, float x, float y) {
    for (auto& [defID, def] : gameDataCopy.entityDefs) {
      if (def.name == name) {
        int id = rand() % 100000;
        gameDataCopy.worldData.entities.push_back({id, defID, x, y});
        return;
      }
    }
  });

  lua.set_function("getButtonHeld", getButtonHeld);

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
    auto result = funcs["update"](&entity);

    if (!result.valid()) {
      sol::error err = result;
      std::cerr << "Error executing script: " << err.what() << '\n';
    }
  }

  render();
}

void Runner::render() {
  SDL_SetRenderTarget(renderer, displayTexture);

  SDL_SetRenderDrawColor(renderer, 0, 0, 10, 255);
  SDL_RenderClear(renderer);

  for (auto& entity : gameDataCopy.worldData.entities) {
    auto& def = entity.entityDefOverride;

    SDL_FRect rect = {
        entity.x - entity.entityDefOverride.width / 2,   //
        entity.y - entity.entityDefOverride.height / 2,  //
        entity.entityDefOverride.width,                  //
        entity.entityDefOverride.height                  //
    };

    if (def.imageMode) {
      SDL_Texture* texture = AssetManager::getInstance().get(renderer, root / def.image);
      SDL_FRect srcRect = {
          def.imageCol,     //
          def.imageRow,     //
          def.imageWidth,   //
          def.imageHeight,  //
      };

      SDL_RenderTextureRotated(renderer, texture, &srcRect, &rect, entity.angle, nullptr, SDL_FLIP_NONE);
    }
    //
    else {
      SDL_SetRenderDrawColor(renderer, entity.entityDefOverride.r, entity.entityDefOverride.g,
                             entity.entityDefOverride.b, entity.entityDefOverride.a);
      SDL_RenderFillRect(renderer, &rect);
    }
  }

  SDL_SetRenderTarget(renderer, nullptr);
}

bool Runner::getButtonHeld(const std::string& button) {
  std::string b = button;
  std::transform(b.begin(), b.end(), b.begin(), ::tolower);

  const bool* k = SDL_GetKeyboardState(NULL);

  if (k[SDL_SCANCODE_SPACE] && (b == "" || b == " ")) return true;
  if (k[SDL_SCANCODE_A] && b == "a") return true;
  if (k[SDL_SCANCODE_B] && b == "b") return true;
  if (k[SDL_SCANCODE_C] && b == "c") return true;
  if (k[SDL_SCANCODE_D] && b == "d") return true;
  if (k[SDL_SCANCODE_E] && b == "e") return true;
  if (k[SDL_SCANCODE_F] && b == "f") return true;
  if (k[SDL_SCANCODE_G] && b == "g") return true;
  if (k[SDL_SCANCODE_H] && b == "h") return true;
  if (k[SDL_SCANCODE_I] && b == "i") return true;
  if (k[SDL_SCANCODE_J] && b == "j") return true;
  if (k[SDL_SCANCODE_K] && b == "k") return true;
  if (k[SDL_SCANCODE_L] && b == "l") return true;
  if (k[SDL_SCANCODE_M] && b == "m") return true;
  if (k[SDL_SCANCODE_N] && b == "n") return true;
  if (k[SDL_SCANCODE_O] && b == "o") return true;
  if (k[SDL_SCANCODE_P] && b == "p") return true;
  if (k[SDL_SCANCODE_Q] && b == "q") return true;
  if (k[SDL_SCANCODE_R] && b == "r") return true;
  if (k[SDL_SCANCODE_S] && b == "s") return true;
  if (k[SDL_SCANCODE_T] && b == "t") return true;
  if (k[SDL_SCANCODE_U] && b == "u") return true;
  if (k[SDL_SCANCODE_V] && b == "v") return true;
  if (k[SDL_SCANCODE_W] && b == "w") return true;
  if (k[SDL_SCANCODE_X] && b == "x") return true;
  if (k[SDL_SCANCODE_Y] && b == "y") return true;
  if (k[SDL_SCANCODE_Z] && b == "z") return true;
  if (k[SDL_SCANCODE_UP] && b == "up") return true;
  if (k[SDL_SCANCODE_DOWN] && b == "down") return true;
  if (k[SDL_SCANCODE_LEFT] && b == "left") return true;
  if (k[SDL_SCANCODE_RIGHT] && b == "right") return true;

  return false;
}

}  // namespace Vania

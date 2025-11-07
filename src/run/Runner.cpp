#include "Runner.hpp"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <print>
#include <string>

#include "GameDataStructs.hpp"
#include "SDL3/SDL_render.h"
#include "panels/EditorContext.hpp"
#include "physics/collision.hpp"
#include "render/RendererForSDL.hpp"
#include "run/AssetManager.hpp"
#include "run/utils.hpp"
#include "sol/forward.hpp"
#include "sol/types.hpp"

namespace Vania {

Runner::Runner(EditorContext& context) : root(context.gameData.editorData.rootPath), context(context) {
  displayTexture = SDL_CreateTexture(context.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
                                     DISPLAY_WIDTH, DISPLAY_HEIGHT);

  lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::os, sol::lib::string, sol::lib::math);

  lua.set_function("summon", [this](const std::string& name, float x, float y) {
    for (auto& [defID, def] : gameDataCopy.entityDefs) {
      if (def.name == name) {
        int id = rand() % 100000;
        gameDataCopy.worldData.entities.push_back({id, defID, x, y});
        Entity& entity = gameDataCopy.worldData.entities.back();
        entity.entityDefOverride = def;
        entity.properties = lua.create_table();
        return &entity;
      }
    }

    return (Entity*)nullptr;
  });

  lua.set_function("getEntitiesByDef", [this](const std::string& defName) {
    std::vector<Entity*> out;

    EntityDef* def = getDefFromName(defName);
    if (def == nullptr) return out;

    for (Entity& entity : gameDataCopy.worldData.entities) {
      if (entity.defID != def->id) continue;
      out.push_back(&entity);
    }

    return out;
  });

  lua.set_function("mousePos", [this]() { return lua.create_table_with("x", mousePosition.x, "y", mousePosition.y); });
  lua.set_function("isColliding", [this](const Entity& a, const Entity& b) { return isColliding(gameDataCopy, a, b); });

  EntityDef::exposeToLua(lua);
  Entity::exposeToLua(lua);
  WorldData::exposeToLua(lua);
  utils::exposeAll(lua);

  reset(context.gameData);
}

Runner::~Runner() { SDL_DestroyTexture(displayTexture); }

void Runner::reset(const GameData& gameData) {
  gameDataCopy = gameData;

  for (auto& entity : gameDataCopy.worldData.entities) {
    const EntityDef& def = gameData.entityDefs.at(entity.defID);
    entity.entityDefOverride = def;
    entity.properties = lua.create_table();
  }

  lastTime = SDL_GetTicks();

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

void Runner::updateMousePos(const SDL_FPoint& mousePos) { mousePosition = mousePos; }

void Runner::update() {
  Uint32 currentTime = SDL_GetTicks();
  float deltaTime = (currentTime - lastTime) / 1000.0f;
  lastTime = currentTime;

  for (auto& entity : gameDataCopy.worldData.entities) {
    const std::string& script = entity.entityDefOverride.script;
    if (script == "") continue;
    sol::table funcs = lua.script_file(root / script);
    auto result = funcs["update"](&entity, deltaTime);

    if (!result.valid()) {
      sol::error err = result;
      std::cerr << "Error executing script: " << err.what() << '\n';
    }
  }

  render();
}

void Runner::render() {
  SDL_SetRenderTarget(context.renderer, displayTexture);

  SDL_SetRenderDrawColor(context.renderer, 0, 0, 10, 255);
  SDL_RenderClear(context.renderer);

  for (auto& entity : gameDataCopy.worldData.entities) {
    auto& def = entity.entityDefOverride;

    RendererForSDL(context).drawEntity(def, {entity.x, entity.y});

    // SDL_FRect rect = {
    //     entity.x - entity.entityDefOverride.width / 2,   //
    //     entity.y - entity.entityDefOverride.height / 2,  //
    //     entity.entityDefOverride.width,                  //
    //     entity.entityDefOverride.height                  //
    // };
    //
    // if (def.imageMode) {
    //   SDL_Texture* texture = AssetManager::getInstance().get(renderer, root / def.image);
    //   SDL_FRect srcRect = {
    //       def.imageCol,     //
    //       def.imageRow,     //
    //       def.imageWidth,   //
    //       def.imageHeight,  //
    //   };
    //
    //   SDL_RenderTextureRotated(renderer, texture, &srcRect, &rect, entity.angle, nullptr, SDL_FLIP_NONE);
    // }
    // //
    // else {
    //   SDL_SetRenderDrawColor(renderer, entity.entityDefOverride.r, entity.entityDefOverride.g,
    //                          entity.entityDefOverride.b, entity.entityDefOverride.a);
    //   SDL_RenderFillRect(renderer, &rect);
    // }
  }

  SDL_SetRenderTarget(context.renderer, nullptr);
}

EntityDef* Runner::getDefFromName(const std::string& name) {
  for (auto& [id, def] : gameDataCopy.entityDefs) {
    if (name == def.name) return &def;
  }

  return nullptr;
}

}  // namespace Vania

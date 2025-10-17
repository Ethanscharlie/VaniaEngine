#include "Runner.hpp"

#include <print>

#include "GameDataStructs.hpp"
#include "SDL3/SDL_render.h"
#include "run/AssetManager.hpp"
#include "sol/forward.hpp"
#include "sol/lua_value.hpp"

namespace Vania {

Runner::Runner(const nlohmann::json& gameData, SDL_Renderer* renderer)
    : root(gameData["editor"]["rootPath"]), renderer(renderer) {
  displayTexture =
      SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, DISPLAY_WIDTH, DISPLAY_HEIGHT);

  lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::table);

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
  gameDataTable = jsonToSol(gameData, lua);

  render();
}

void Runner::runAllScriptsSetups() {
  // int size = gameDataTable["entities"]["size"];
  // for (int i = 0; i < size; i++) {
  //   auto entity = gameDataTable["entities"][i];
  //   const std::string id = entity["defId"];
  //   const auto& defs = gameDataTable["defs"][id];
  //   const std::string& script = defs["script"];
  //   if (script == "") continue;
  //   sol::table funcs = lua.script_file(root / script);
  //   auto result = funcs["setup"](&entity);
  //   if (!result.valid()) {
  //     sol::error err = result;
  //     std::cout << "Lua error: " << err.what() << std::endl;
  //   }
  // }
}

void Runner::update() {
  // int size = gameDataTable["entities"]["size"];
  // for (int i = 0; i < size; i++) {
  //   auto entity = gameDataTable["entities"][i];
  //   const std::string id = entity["defId"];
  //   const auto& defs = gameDataTable["defs"][id];
  //   const std::string& script = defs["script"];
  //   if (script == "") continue;
  //   sol::table funcs = lua.script_file(root / script);
  //   funcs["update"](&entity);
  //
  //   const bool* keystates = SDL_GetKeyboardState(NULL);
  //   if (keystates[SDL_SCANCODE_SPACE]) {
  //     funcs["keyHeldSpace"](&entity);
  //   }
  //   if (keystates[SDL_SCANCODE_RIGHT] || keystates[SDL_SCANCODE_D]) {
  //     funcs["keyHeldRight"](&entity);
  //   }
  //   if (keystates[SDL_SCANCODE_LEFT] || keystates[SDL_SCANCODE_A]) {
  //     funcs["keyHeldLeft"](&entity);
  //   }
  //   if (keystates[SDL_SCANCODE_UP] || keystates[SDL_SCANCODE_W]) {
  //     funcs["keyHeldUp"](&entity);
  //   }
  //   if (keystates[SDL_SCANCODE_DOWN] || keystates[SDL_SCANCODE_S]) {
  //     funcs["keyHeldDown"](&entity);
  //   }
  // }
  //
  // render();
}

void Runner::render() {
  SDL_SetRenderTarget(renderer, displayTexture);

  SDL_SetRenderDrawColor(renderer, 0, 0, 10, 255);
  SDL_RenderClear(renderer);

  for (auto& pair : gameDataTable["entities"].get<sol::table>()) {
    auto entity = pair.second.as<sol::table>();
    const std::string id = entity["defId"];
    // const auto& defs = gameDataTable["defs"][id];
    // SDL_SetRenderDrawColor(renderer, defs["r"], defs["g"], defs["b"], defs["a"]);
    // SDL_FRect rect = {entity["x"], entity["y"], (float)defs["width"], (float)defs["height"]};
    // SDL_RenderFillRect(renderer, &rect);
    //
    // SDL_Texture* texture = AssetManager::getInstance().get(renderer, root / "img.png");
    // SDL_RenderTexture(renderer, texture, nullptr, &rect);
  }

  SDL_SetRenderTarget(renderer, nullptr);
}

sol::table Runner::jsonToSol(const nlohmann::json& value, sol::state& lua) {
  using nlohmann::json;

  switch (value.type()) {
    case json::value_t::object: {
      auto table = lua.create_table(0, static_cast<int>(value.size()));
      for (const auto& [k, v] : value.items()) {
        table[k] = jsonToSol(v, lua);
      }

      return sol::make_object(lua, table);
    }

    case json::value_t::array: {
      auto table = lua.create_table(static_cast<int>(value.size()), 0);
      for (auto i = 0uz, n = value.size(); i < n; ++i) {
        table[i + 1] = jsonToSol(value[i], lua);
      }

      return sol::make_object(lua, lua);
    }

    case json::value_t::string:
      return sol::make_object(lua, value.get<std::string>());

    case json::value_t::boolean:
      return sol::make_object(lua, value.get<bool>());

    case json::value_t::number_integer:
      return sol::make_object(lua, value.get<int64_t>());

    case json::value_t::number_unsigned:
      return sol::make_object(lua, value.get<uint64_t>());

    case json::value_t::number_float:
      return sol::make_object(lua, value.get<double>());

    default:
      return sol::make_object(lua, sol::lua_nil);
  }
}

}  // namespace Vania

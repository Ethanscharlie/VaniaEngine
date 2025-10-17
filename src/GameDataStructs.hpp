#pragma once

#include <filesystem>
#include <map>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <numbers>
#include <string>
#include <vector>

#include "nlohmann/detail/macro_scope.hpp"
#include "sol/state.hpp"

struct Property {
  float number;
  std::string text;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(  //
    Property,                        //
    number,                          //
    text                             //
)

struct EntityDef {
  int id = rand() % 100000001;
  std::string name = "New Entity";

  float width = 16;
  float height = 16;

  int r = 255;
  int g = 255;
  int b = 255;
  int a = 255;
  std::string image;
  bool imageMode = false;

  std::string script = "";
  std::map<std::string, Property> properties;

  static void exposeToLua(sol::state& lua) {
    lua.new_usertype<EntityDef>(       //
        "EntityDef",                   //
        "id", &EntityDef::id,          //
        "name", &EntityDef::name,      //
        "width", &EntityDef::width,    //
        "height", &EntityDef::height,  //
        "r", &EntityDef::r,            //
        "g", &EntityDef::g,            //
        "b", &EntityDef::b,            //
        "a", &EntityDef::a,            //
        "image", &EntityDef::image,    //
        "script", &EntityDef::script   //
    );
  }
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(  //
    EntityDef,                       //
    id,                              //
    name,                            //
    width,                           //
    height,                          //
    r,                               //
    g,                               //
    b,                               //
    a,                               //
    image,                           //
    script,                          //
    properties                       //
)

struct Entity {
  EntityDef* entityDef;
  int defID;
  float x;
  float y;

  static void exposeToLua(sol::state& lua) {
    lua.new_usertype<Entity>(       //
        "Entity",                   //
        "def", &Entity::entityDef,  //
        "x", &Entity::x,            //
        "y", &Entity::y             //
    );
  }
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(  //
    Entity,                          //
    defID,                           //
    x,                               //
    y                                //
)

struct WorldData {
  std::vector<Entity> entities;

  static void exposeToLua(sol::state& lua) {}
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(  //
    WorldData,                       //
    entities                         //
)

struct EditorData {
  EntityDef* selectedEntityDef = nullptr;
  bool entityDefEditorOpen = false;
  std::filesystem::path rootPath = "../testres/";
};

struct GameData {
  std::vector<EntityDef> entityDefs;

  WorldData worldData;
  EditorData editorData;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(  //
    GameData,                        //
    entityDefs,                      //
    worldData                        //
)

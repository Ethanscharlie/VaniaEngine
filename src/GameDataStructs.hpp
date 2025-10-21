#pragma once

#include <filesystem>
#include <map>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <numbers>
#include <print>
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
  int id = 0;
  std::string name = "New Entity";

  float width = 16;
  float height = 16;

  int r = 255;
  int g = 255;
  int b = 255;
  int a = 255;

  float imageWidth = 0;
  float imageHeight = 0;
  float imageRow = 0;
  float imageCol = 0;
  std::string image = "";
  bool imageMode = false;

  std::string script = "";
  std::map<std::string, Property> properties;

  static void exposeToLua(sol::state& lua) {
    lua.new_usertype<EntityDef>(                 //
        "EntityDef",                             //
        "id", &EntityDef::id,                    //
        "name", &EntityDef::name,                //
        "width", &EntityDef::width,              //
        "height", &EntityDef::height,            //
        "r", &EntityDef::r,                      //
        "g", &EntityDef::g,                      //
        "b", &EntityDef::b,                      //
        "a", &EntityDef::a,                      //
        "imageWith", &EntityDef::imageWidth,     //
        "imageHeight", &EntityDef::imageHeight,  //
        "imageRow", &EntityDef::imageRow,        //
        "imageCol", &EntityDef::imageCol,        //
        "image", &EntityDef::image,              //
        "script", &EntityDef::script             //
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
    imageWidth,                      //
    imageHeight,                     //
    imageRow,                        //
    imageCol,                        //
    imageMode,                       //
    image,                           //
    script,                          //
    properties                       //
)

struct Entity {
  int id = 0;
  int defID = 0;
  EntityDef entityDefOverride;
  float x;
  float y;
  float angle = 0;

  Entity() {}
  Entity(int id, int entityDefID, float x, float y) : id(id), defID(entityDefID), x(x), y(y) {}

  bool operator==(Entity const& other) const { return this->id == other.id; }

  static void exposeToLua(sol::state& lua) {
    lua.new_usertype<Entity>(               //
        "Entity",                           //
        "def", &Entity::entityDefOverride,  //
        "id", &Entity::id,                  //
        "angle", &Entity::angle,            //
        "x", &Entity::x,                    //
        "y", &Entity::y                     //
    );
  }
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(  //
    Entity,                          //
    defID,                           //
    id,                              //
    x,                               //
    y                                //
)

struct WorldData {
  std::vector<Entity> entities;
  int gridSize = 32;

  static void exposeToLua(sol::state& lua) {}
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(  //
    WorldData,                       //
    entities,                        //
    gridSize                         //
)

struct EditorData {
  EntityDef* selectedEntityDef = nullptr;
  bool entityDefEditorOpen = false;
  std::filesystem::path rootPath = "../testres/";
};

struct GameData {
  std::map<int, EntityDef> entityDefs;

  WorldData worldData;
  EditorData editorData;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(  //
    GameData,                        //
    entityDefs,                      //
    worldData                        //
)

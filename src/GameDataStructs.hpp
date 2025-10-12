#pragma once

#include <filesystem>
#include <string>
#include <vector>

struct EntityDef {
  std::string name = "New Entity";
  float width = 16;
  float height = 16;
  std::string color = "white";
  std::string script = "myscript.lua";
};

struct Entity {
  EntityDef *entityDef;
  float x;
  float y;
};

struct WorldData {
  std::vector<Entity> entities;
};

struct EditorData {
  EntityDef *selectedEntityDef = nullptr;
  bool entityDefEditorOpen = false;
  std::filesystem::path rootPath = "../testres/";
};

struct GameData {
  std::vector<EntityDef> entityDefs;

  WorldData worldData;
  EditorData editorData;
};

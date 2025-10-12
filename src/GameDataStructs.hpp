#pragma once

#include <string>
#include <vector>

struct EntityDef {
  std::string name = "New Entity";
  int width = 16;
  int height = 16;
  std::string color = "white";
};

struct Entity {
  EntityDef *entityDef;
  int x;
  int y;
};

struct WorldData {
  std::vector<Entity> entities;
};

struct EditorData {
  EntityDef *selectedEntityDef = nullptr;
  bool entityDefEditorOpen = false;
};

struct GameData {
  std::vector<EntityDef> entityDefs;

  WorldData worldData;
  EditorData editorData;
};

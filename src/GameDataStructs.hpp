#pragma once

#include <string>
#include <vector>

struct EntityDef {
  std::string name = "New Entity";
  int width = 16;
  int height = 16;
  std::string color = "white";
};

struct EditorData {
  int selectedEntityDef = 0;
};

struct GameData {
  std::vector<EntityDef> entityDefs;

  EditorData editorData;
};

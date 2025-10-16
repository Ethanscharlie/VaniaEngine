#pragma once

#include <nlohmann/json_fwd.hpp>

#include "nlohmann/json.hpp"

inline nlohmann::json createEntityDef() {
  nlohmann::json node;
  node["name"] = "New Entity";
  node["width"] = 16;
  node["height"] = 16;
  node["r"] = 255;
  node["g"] = 255;
  node["b"] = 255;
  node["a"] = 255;
  node["image"] = "";
  node["imageMode"] = false;
  node["script"] = "";
  return node;
}

inline nlohmann::json createEntity(std::string defId, float x, float y) {
  nlohmann::json node;
  node["defId"] = defId;
  node["x"] = x;
  node["y"] = y;
  return node;
}

inline nlohmann::json createGameData() {
  nlohmann::json node;
  node["defs"];
  node["entities"];

  nlohmann::json editor;
  editor["selectedDef"] = "";
  editor["entityDefEditorOpen"] = false;
  editor["rootPath"] = "../testres/";
  node["editor"] = editor;

  return node;
}

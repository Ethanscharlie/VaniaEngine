#include "EntityPanel.hpp"

#include <cstddef>
#include <format>
#include <iostream>
#include <nlohmann/json_fwd.hpp>
#include <print>

#include "GameDataStructs.hpp"
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"

namespace Vania {
EntityPanel::EntityPanel(nlohmann::json& gameData) : gameData(gameData) {}

void EntityPanel::update() {
  ImGui::Begin("Entity");

  if (ImGui::Button("   +   ")) {
    srand(time(0));
    gameData["defs"][std::to_string(rand() % 100001)] = createEntityDef();
  }

  {
    ImGui::BeginChild("Entity Selection", ImVec2(150, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);

    for (auto& [defID, entity] : gameData["defs"].items()) {
      std::string name = entity["name"];
      const std::string label = std::format("{}###{}", name, defID);

      std::string selectedDefID = gameData["editor"]["selectedDef"];
      bool isSelected = selectedDefID == defID;
      if (ImGui::Selectable(label.c_str(), isSelected, ImGuiSelectableFlags_SelectOnNav)) {
        gameData["editor"]["selectedDef"] = defID;
      }
    }

    ImGui::EndChild();
  }

  ImGui::SameLine();

  const bool somethingIsSelected = gameData["editor"]["selectedDef"] != "";
  if (ImGui::Button("Edit") && somethingIsSelected) {
    ImGui::OpenPopup("Entity Def Editor");
  }

  bool defEditorOpen = true;
  if (ImGui::BeginPopupModal("Entity Def Editor", &defEditorOpen)) {
    showPropertyEditor();
    ImGui::EndPopup();
  }

  ImGui::End();
}

void EntityPanel::showPropertyEditor() {
  static int SMALL_NUMBER_WIDTH = 100;
  std::string selectedDefID = gameData["editor"]["selectedDef"];

  ImGui::BeginGroup();

  static std::string name = gameData["defs"][selectedDefID]["name"];
  if (ImGui::InputText("name", &name)) gameData["defs"][selectedDefID]["name"] = name;

  ImGui::SetNextItemWidth(SMALL_NUMBER_WIDTH);
  static float width = gameData["defs"][selectedDefID]["width"];
  if (ImGui::InputFloat("Width", &width)) gameData["defs"][selectedDefID]["width"] = width;

  ImGui::SetNextItemWidth(SMALL_NUMBER_WIDTH);
  static float height = gameData["defs"][selectedDefID]["height"];
  if (ImGui::InputFloat("Height", &height)) gameData["defs"][selectedDefID]["height"] = height;

  static bool imageMode = gameData["defs"][selectedDefID]["imageMode"];
  std::cout << imageMode << "\n";
  ImGui::SetNextItemWidth(SMALL_NUMBER_WIDTH);
  if (ImGui::BeginCombo("###renderCombo", imageMode ? "image" : "box")) {
    if (ImGui::Selectable("box", !imageMode)) {
      gameData["defs"][selectedDefID]["imageMode"] = false;
    }
    if (ImGui::Selectable("image", imageMode)) {
      gameData["defs"][selectedDefID]["imageMode"] = true;
    }
    ImGui::EndCombo();
  }

  ImGui::SameLine();
  if (imageMode) {
    static std::string image = gameData["defs"][selectedDefID]["image"];
    if (ImGui::InputText("Image", &image)) gameData["defs"][selectedDefID]["image"] = image;
  } else {
    static int r = gameData["defs"][selectedDefID]["r"];
    static int g = gameData["defs"][selectedDefID]["g"];
    static int b = gameData["defs"][selectedDefID]["b"];
    static int a = gameData["defs"][selectedDefID]["a"];
    static float color_f[4] = {r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f};
    if (ImGui::ColorEdit4("###Color", color_f, ImGuiColorEditFlags_NoInputs)) {
      gameData["defs"][selectedDefID]["r"] = color_f[0] * 255.0f;
      gameData["defs"][selectedDefID]["g"] = color_f[1] * 255.0f;
      gameData["defs"][selectedDefID]["b"] = color_f[2] * 255.0f;
      gameData["defs"][selectedDefID]["a"] = color_f[3] * 255.0f;
    }
  }

  static std::string script = gameData["defs"][selectedDefID]["script"];
  if (ImGui::InputText("Script", &script)) gameData["defs"][selectedDefID]["script"] = script;

  ImGui::EndGroup();
}

}  // namespace Vania

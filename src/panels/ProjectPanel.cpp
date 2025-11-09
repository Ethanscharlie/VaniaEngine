#include "ProjectPanel.hpp"

#include <fstream>

#include "imgui.h"

namespace Vania {

ProjectPanel::ProjectPanel(EditorContext& context) : context(context) {}

void ProjectPanel::update() {
  ImGui::Begin("Project");

  if (ImGui::Button("Save")) {
    const std::filesystem::path mainPath = context.gameData.editorData.rootPath / "main.json";
    const nlohmann::json j = context.gameData;
    std::ofstream file(mainPath);
    file << j.dump(4);
    file.close();
  }

  ImGui::End();
}

}  // namespace Vania

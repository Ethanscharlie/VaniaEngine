#include "ProjectPanel.hpp"

#include <fstream>
#include <string>

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

  std::string& backgroundImage = context.gameData.backgroundImage;
  if (ImGui::BeginCombo("BackgroundImageSelectionCombo", backgroundImage.c_str())) {
    if (ImGui::Selectable("None")) {
      backgroundImage = "";
    }

    const auto& images = context.filesystemWatcher.getAllFilesWithExtension(".png");
    for (const auto& image : images) {
      if (ImGui::Selectable(image.c_str(), image == backgroundImage)) {
        backgroundImage = image;
      }
    }
    ImGui::EndCombo();
  }

  ImGui::SliderFloat("Camera Zoom", &context.gameData.cameraZoom, 0.2, 4);

  ImGui::End();
}

}  // namespace Vania

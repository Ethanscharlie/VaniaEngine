#include "ProjectPanel.hpp"

#include "imgui.h"

namespace Vania {

ProjectPanel::ProjectPanel(EditorContext& context) : context(context) {}

void ProjectPanel::update() {
  ImGui::Begin("Project");
  ImGui::End();
}

}  // namespace Vania

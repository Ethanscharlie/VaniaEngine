#include "ConsolePanel.hpp"

#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"

namespace Vania {

std::string ConsolePanel::consoleOutput = "";

ConsolePanel::ConsolePanel(EditorContext& context) : context(context) {}

void ConsolePanel::update() {
  ImGui::Begin("Console");
  ImGui::InputTextMultiline("##consolemultiline", &consoleOutput, {1000, 400}, ImGuiInputTextFlags_ReadOnly);
  ImGui::End();
}

}  // namespace Vania

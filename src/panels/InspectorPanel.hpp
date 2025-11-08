
#include <filesystem>

#include "imgui.h"
#include "panels/EditorContext.hpp"
#include "panels/IPanel.hpp"
#include "render/RendererForImGui.hpp"

namespace Vania {
class InspectorPanel : public IPanel {
  const int SMALL_NUMBER_WIDTH = 100;

  EditorContext& context;

  // Image Picker
  int cellWidth = 1;
  int cellHeight = 1;

  // script creator
  std::string scriptCreatorTempText = "";

  // Collision
  ImDrawList* draw_list;
  RendererForImGui vaniaRenderer;

 public:
  InspectorPanel(EditorContext& context);
  void update() override;
  void showSample();
  void showGraphic();
  void showBehaviour();
  void showCollision();
  void showPropertyEditor();
  void showImagePicker();
  void showScriptCreator();
  void createScriptAndAttach(const std::filesystem::path& newScript);
};
}  // namespace Vania

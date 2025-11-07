
#include <filesystem>

#include "imgui.h"
#include "panels/EditorContext.hpp"
#include "panels/IPanel.hpp"

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

 public:
  InspectorPanel(EditorContext& context);
  void update() override;
  void showGraphic();
  void showBehaviour();
  void showCollision();
  void showPropertyEditor();
  void showImagePicker();
  void showScriptCreator();
  void createScriptAndAttach(const std::filesystem::path& newScript);
};
}  // namespace Vania

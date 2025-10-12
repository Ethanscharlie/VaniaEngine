#include "GameDataStructs.hpp"
#include "IPanel.hpp"
#include "imgui.h"
#include <vector>

namespace Vania {
class WorldPanel : public IPanel {
  const float GRID_STEP = 64.0f;
  const ImU32 DARK_GRAY = IM_COL32(50, 50, 50, 255);
  const ImU32 LIGHT_GRAY = IM_COL32(200, 200, 200, 40);
  const ImU32 WHITE = IM_COL32(255, 255, 255, 255);

  GameData &gameData;

  ImVec2 scrolling = {0.0f, 0.0f};

  ImVec2 canvas_p0;
  ImVec2 canvas_sz;
  ImVec2 canvas_p1;

  ImDrawList *draw_list;

public:
  WorldPanel(GameData &gameData);
  void update() override;

private:
  void calculateCanvasPositionValues();
  ImVec2 getMousePositionOnCanvas();
  ImVec2 getOrigin();

  void drawGrid();
  void draw();
};
} // namespace Vania

#include "GameDataStructs.hpp"
#include "IPanel.hpp"
#include "imgui.h"
#include <vector>

namespace Vania {
class WorldPanel : public IPanel {
  GameData &gameData;

  ImVec2 scrolling = {0.0f, 0.0f};

  ImVec2 canvas_p0;
  ImVec2 canvas_sz;
  ImVec2 canvas_p1;

  bool is_hovered;
  bool is_active;
  ImVec2 origin;
  ImVec2 mouse_pos_in_canvas;

public:
  WorldPanel(GameData &gameData);
  void update() override;

private:
  void calculateCanvasPositionValues();
  void findWindowInputState();
  void draw();
};
} // namespace Vania

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

public:
  WorldPanel(GameData &gameData);
  void update() override;

private:
  void calculateCanvasPositionValues();
  ImVec2 getMousePositionOnCanvas();
  void draw();
};
} // namespace Vania

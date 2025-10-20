#include <vector>

#include "GameDataStructs.hpp"
#include "IPanel.hpp"
#include "SDL3/SDL_render.h"
#include "imgui.h"

namespace Vania {
class WorldPanel : public IPanel {
  const ImU32 DARK_GRAY = IM_COL32(50, 50, 50, 255);
  const ImU32 LIGHT_GRAY = IM_COL32(200, 200, 200, 40);
  const ImU32 WHITE = IM_COL32(255, 255, 255, 255);
  const ImU32 RED = IM_COL32(255, 0, 0, 255);
  const float HOVER_BOX_PADDING = 5;

  GameData& gameData;
  SDL_Renderer* renderer;

  ImVec2 scrolling = {0.0f, 0.0f};
  float zoom = 1;

  ImVec2 canvas_p0;
  ImVec2 canvas_sz;
  ImVec2 canvas_p1;

  ImDrawList* draw_list;

 public:
  WorldPanel(GameData& gameData, SDL_Renderer* renderer);
  void update() override;

 private:
  void calculateCanvasPositionValues();
  ImVec2 getMousePositionOnCanvas();
  ImVec2 getOrigin();
  Entity* isHoveringOverEntity();
  void whileActive();
  ImVec4 getEntityMinAndMax(const Entity& entity);
  void drawHoverBox(const Entity& entity);
  void drawGrid();
  void draw();
  void drawBox(const ImVec2& min, const ImVec2& max, int r, int g, int b, int a);
  void drawNoImage(const ImVec2& min, const ImVec2& max);
  float snapPositionToGrid(float x);
  void createEntity(float x, float y);
};
}  // namespace Vania

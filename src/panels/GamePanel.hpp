#include "GameDataStructs.hpp"
#include "IPanel.hpp"
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
#include "imgui.h"
#include "panels/EditorContext.hpp"
#include "run/Runner.hpp"

namespace Vania {
class GamePanel : public IPanel {
  EditorContext& context;
  Runner runner;

  ImVec2 gameWindowSize = {0, 0};

  bool running = false;

 public:
  GamePanel(EditorContext& context);
  void update() override;

 private:
  SDL_FPoint getMousePosOnGame();
};
}  // namespace Vania

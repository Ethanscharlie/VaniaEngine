#include "GameDataStructs.hpp"
#include "IPanel.hpp"
#include "SDL3/SDL_render.h"
#include "run/Runner.hpp"

namespace Vania {
class GamePanel : public IPanel {
  GameData &gameData;
  Runner runner;

  bool running = false;

public:
  GamePanel(GameData &gameData, SDL_Renderer *renderer);
  void update() override;
};
} // namespace Vania

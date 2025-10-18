#include "GameDataStructs.hpp"
#include "IPanel.hpp"
#include "SDL3/SDL_render.h"

namespace Vania {
class AssetPanel : public IPanel {
  GameData& gameData;
  SDL_Renderer* renderer;

  float zoom = 1;
  int gridSize = 16;

 public:
  AssetPanel(GameData& gameData, SDL_Renderer* renderer);
  void update() override;
};
}  // namespace Vania

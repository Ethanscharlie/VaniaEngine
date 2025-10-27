#include <vector>

#include "GameDataStructs.hpp"
#include "IPanel.hpp"
#include "SDL3/SDL_render.h"

namespace Vania {
class EntityPanel : public IPanel {
  const int SMALL_NUMBER_WIDTH = 100;

  GameData &gameData;
  SDL_Renderer *renderer;

public:
  EntityPanel(GameData &gameData, SDL_Renderer *renderer);
  void update() override;

private:
  EntityDef &createDef();
};
} // namespace Vania

#include <vector>

#include "GameDataStructs.hpp"
#include "IPanel.hpp"
#include "SDL3/SDL_render.h"

namespace Vania {
class EntityPanel : public IPanel {
  const int SMALL_NUMBER_WIDTH = 100;

  GameData& gameData;
  SDL_Renderer* renderer;

  // Image Picker
  int cellWidth = 1;
  int cellHeight = 1;

 public:
  EntityPanel(GameData& gameData, SDL_Renderer* renderer);
  void update() override;

 private:
  void showPropertyEditor();
  void showImagePicker();
  EntityDef& createDef();
};
}  // namespace Vania

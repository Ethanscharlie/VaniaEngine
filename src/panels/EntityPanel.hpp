#include <vector>

#include "FilesystemWatcher.hpp"
#include "GameDataStructs.hpp"
#include "IPanel.hpp"
#include "SDL3/SDL_render.h"

namespace Vania {
class EntityPanel : public IPanel {
  const int SMALL_NUMBER_WIDTH = 100;

  GameData &gameData;
  SDL_Renderer *renderer;
  FileSystemWatcher &filesystemWatcher;

public:
  EntityPanel(GameData &gameData, SDL_Renderer *renderer,
              FileSystemWatcher &filesystemWatcher);
  void update() override;

private:
  EntityDef &createDef();
};
} // namespace Vania

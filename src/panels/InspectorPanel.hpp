
#include "FilesystemWatcher.hpp"
#include "GameDataStructs.hpp"
#include "SDL3/SDL_render.h"
#include "panels/IPanel.hpp"

namespace Vania {
class InspectorPanel : public IPanel {
  const int SMALL_NUMBER_WIDTH = 100;

  GameData& gameData;
  SDL_Renderer* renderer;
  FileSystemWatcher& filesystemWatcher;

  // Image Picker
  int cellWidth = 1;
  int cellHeight = 1;

 public:
  InspectorPanel(GameData& gameData, SDL_Renderer* renderer, FileSystemWatcher& filesystemWatcher);
  void update() override;
  void showPropertyEditor();
  void showImagePicker();
};
}  // namespace Vania

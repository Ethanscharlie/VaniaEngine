#include "GameDataStructs.hpp"
#include "IPanel.hpp"

namespace Vania {
class EntityDefEditor : public IPanel {
  GameData &gameData;

public:
  EntityDefEditor(GameData &gameData);
  void update() override;

private:
  void showPropertyEditor();
};
} // namespace Vania

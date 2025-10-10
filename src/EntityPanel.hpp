#include "GameDataStructs.hpp"
#include "IPanel.hpp"
#include <vector>

namespace Vania {
class EntityPanel : public IPanel {
  GameData &gameData;

public:
  EntityPanel(GameData &gameData);
  void update() override;
};
} // namespace Vania

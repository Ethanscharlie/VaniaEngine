#include "GameDataStructs.hpp"
#include "IPanel.hpp"
#include <vector>

namespace Vania {
class WorldPanel : public IPanel {
  GameData &gameData;

public:
  WorldPanel(GameData &gameData);
  void update() override;
};
} // namespace Vania

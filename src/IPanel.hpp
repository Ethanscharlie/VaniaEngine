#pragma once

namespace Vania {
class IPanel {
public:
  virtual void update() = 0;
  virtual ~IPanel() {}
};
} // namespace Vania

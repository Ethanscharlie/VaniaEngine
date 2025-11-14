#pragma once

#include "IPanel.hpp"
#include "panels/EditorContext.hpp"

namespace Vania {

class ConsolePanel : public IPanel {
  EditorContext& context;

 public:
  static std::string consoleOutput;

  ConsolePanel(EditorContext& context);
  void update() override;
};

}  // namespace Vania

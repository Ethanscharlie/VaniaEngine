#include <SDL3/SDL.h>
#include <unistd.h>

#include <memory>
#include <vector>

#include "GameDataStructs.hpp"
#include "IPanel.hpp"
#include "imgui.h"
#include "imgui_impl_sdl3.h"

namespace Vania {
class App {
  const int WINDOW_WIDTH = 1920;
  const int WINDOW_HEIGHT = 1080;

  SDL_Window* window = nullptr;
  SDL_Renderer* renderer = nullptr;
  SDL_Event event;
  ImGuiIO* io;

  bool running = true;

  std::vector<std::unique_ptr<IPanel>> panels;

  GameData gameData;

 public:
  App();
  ~App();
  void loadFromFile();
  void update();
  bool isRunning();

 private:
  void startNewFrame();
  void render();
  void initSDL();
  void createWindow();
  void initImGui();
};
};  // namespace Vania

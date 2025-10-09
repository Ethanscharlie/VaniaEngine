#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include <SDL3/SDL.h>
#include <unistd.h>

namespace Vania {
class App {
  const int WINDOW_WIDTH = 1920;
  const int WINDOW_HEIGHT = 1080;

  SDL_Window *window = nullptr;
  SDL_Renderer *renderer = nullptr;
  SDL_Event event;
  ImGuiIO *io;

  bool running = true;

public:
  App();
  ~App();
  void update();
  bool isRunning();

private:
  void startNewFrame();
  void render();
  void initSDL();
  void createWindow();
  void initImGui();
};
}; // namespace Vania

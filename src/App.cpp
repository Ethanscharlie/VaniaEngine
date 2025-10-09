#include "App.hpp"

#include "imgui_impl_sdlrenderer3.h"
#include <format>
#include <stdexcept>

namespace Vania {
App::App() {
  initSDL();
  createWindow();
  initImGui();
}

App::~App() {
  ImGui_ImplSDLRenderer3_Shutdown();
  ImGui_ImplSDL3_Shutdown();
  ImGui::DestroyContext();

  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();
}

void App::update() {
  SDL_PollEvent(&event);
  ImGui_ImplSDL3_ProcessEvent(&event);

  if (event.type == SDL_EVENT_QUIT) {
    running = false;
  }

  startNewFrame();

  ImGui::ShowDemoWindow();

  render();
}

bool App::isRunning() { return running; }

void App::startNewFrame() {
  ImGui_ImplSDLRenderer3_NewFrame();
  ImGui_ImplSDL3_NewFrame();
  ImGui::NewFrame();
}

void App::render() {
  ImGui::Render();
  SDL_SetRenderScale(renderer, io->DisplayFramebufferScale.x,
                     io->DisplayFramebufferScale.y);
  SDL_RenderClear(renderer);
  ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
  SDL_RenderPresent(renderer);
}

void App::initSDL() {
  const int sdlInitSuccess = SDL_Init(SDL_INIT_VIDEO);
  if (!sdlInitSuccess) {
    throw std::runtime_error(
        std::format("Couldn't initialize SDL: %s\n", SDL_GetError()).c_str());
  }
}

void App::createWindow() {
  const int windowCreationSucess =
      SDL_CreateWindowAndRenderer("Demo", WINDOW_WIDTH, WINDOW_HEIGHT,
                                  SDL_WINDOW_RESIZABLE, &window, &renderer);
  if (!windowCreationSucess) {
    throw std::runtime_error(
        std::format("Couldn't initialize SDL window: %s\n", SDL_GetError())
            .c_str());
  }
}

void App::initImGui() {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  io = &ImGui::GetIO();
  (void)io;

  ImGui::StyleColorsDark();

  ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
  ImGui_ImplSDLRenderer3_Init(renderer);
}

}; // namespace Vania

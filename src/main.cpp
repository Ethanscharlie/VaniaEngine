#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"
#include <SDL3/SDL.h>
#include <print>
#include <stdexcept>
#include <unistd.h>

#define WINDOW_WIDTH (int)1000
#define WINDOW_HEIGHT (int)1000

SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;

int main() {
  const int sdlInitSuccess = SDL_Init(SDL_INIT_VIDEO);
  if (!sdlInitSuccess) {
    throw std::runtime_error(
        std::format("Couldn't initialize SDL: %s\n", SDL_GetError()).c_str());
  }

  const int windowCreationSucess =
      SDL_CreateWindowAndRenderer("Demo", WINDOW_WIDTH, WINDOW_HEIGHT,
                                  SDL_WINDOW_RESIZABLE, &window, &renderer);
  if (!windowCreationSucess) {
    throw std::runtime_error(
        std::format("Couldn't initialize SDL window: %s\n", SDL_GetError())
            .c_str());
  }

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;

  ImGui::StyleColorsDark();

  ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
  ImGui_ImplSDLRenderer3_Init(renderer);

  SDL_Event event;
  while (1) {
    SDL_PollEvent(&event);
    ImGui_ImplSDL3_ProcessEvent(&event);

    if (event.type == SDL_EVENT_QUIT) {
      break;
    }

    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    ImGui::ShowDemoWindow();

    ImGui::Render();
    SDL_SetRenderScale(renderer, io.DisplayFramebufferScale.x,
                       io.DisplayFramebufferScale.y);
    SDL_RenderClear(renderer);
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
    SDL_RenderPresent(renderer);
  }

  ImGui_ImplSDLRenderer3_Shutdown();
  ImGui_ImplSDL3_Shutdown();
  ImGui::DestroyContext();

  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();
}

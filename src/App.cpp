#include "App.hpp"

#include <filesystem>
#include <format>
#include <fstream>
#include <functional>
#include <memory>
#include <print>
#include <stdexcept>

#include "GameDataStructs.hpp"
#include "SDL3/SDL_render.h"
#include "imgui.h"
#include "imgui_impl_sdlrenderer3.h"
#include "panels/EntityPanel.hpp"
#include "panels/GamePanel.hpp"
#include "panels/InspectorPanel.hpp"
#include "panels/WorldPanel.hpp"

#define DEFAULT_ROOT_PATH std::filesystem::path("../testres/")

namespace Vania {
App::App() {
  loadFromFile();

  initSDL();
  createWindow();
  initImGui();

  context.window = window;
  context.renderer = renderer;

  panels.emplace_back(std::make_unique<EntityPanel>(context.gameData, renderer, context.filesystemWatcher));
  panels.emplace_back(std::make_unique<WorldPanel>(context));
  panels.emplace_back(std::make_unique<GamePanel>(context));
  panels.emplace_back(std::make_unique<InspectorPanel>(context));
}

App::~App() {
  ImGui_ImplSDLRenderer3_Shutdown();
  ImGui_ImplSDL3_Shutdown();
  ImGui::DestroyContext();

  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();
}

void App::loadFromFile() {
  std::filesystem::path mainPath = DEFAULT_ROOT_PATH / "main.json";

  if (!std::filesystem::exists(mainPath)) {
    std::println("{} path doesnt exist", mainPath.string());
  }

  std::ifstream ifs(mainPath);
  try {
    nlohmann::json gameDataJson;
    ifs >> gameDataJson;

    if (gameDataJson.is_object()) {
      context.gameData = gameDataJson.get<GameData>();
      return;
    } else {
      std::cerr << "Error: Invalid JSON structure in file: " << mainPath << std::endl;
    }
  } catch (const nlohmann::json::parse_error& e) {
    std::cerr << "Error: Failed to parse JSON. " << e.what() << std::endl;
  } catch (const nlohmann::json::out_of_range& e) {
    std::cerr << "Error: Failed to parse JSON. " << e.what() << std::endl;
  }

  // Template project
  int id = rand() % 100000;
  EntityDef newDef;
  newDef.id = id;
  context.gameData.entityDefs[id] = newDef;
  newDef.name = "Player";
  newDef.script = "player.lua";

  int def1id = 0;
  for (auto& [id, def] : context.gameData.entityDefs) {
    def1id = id;
    break;
  }

  context.gameData.worldData.entities.push_back({101, def1id, 0, 0});
}

void App::update() {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  SDL_PollEvent(&event);
  ImGui_ImplSDL3_ProcessEvent(&event);
  if (event.type == SDL_EVENT_QUIT) {
    running = false;
  }

  startNewFrame();

  ImGui::DockSpaceOverViewport();

  ImGui::ShowDemoWindow();

  for (auto& panel : panels) {
    panel->update();
  }

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
  SDL_SetRenderScale(renderer, io->DisplayFramebufferScale.x, io->DisplayFramebufferScale.y);
  ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);

  for (std::function<void()> f : context.postImGuiRender) f();
  context.postImGuiRender.clear();

  SDL_RenderPresent(renderer);
}

void App::initSDL() {
  const int sdlInitSuccess = SDL_Init(SDL_INIT_VIDEO);
  if (!sdlInitSuccess) {
    throw std::runtime_error(std::format("Couldn't initialize SDL: %s\n", SDL_GetError()).c_str());
  }
}

void App::createWindow() {
  const int windowCreationSucess =
      SDL_CreateWindowAndRenderer("Demo", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer);
  if (!windowCreationSucess) {
    throw std::runtime_error(std::format("Couldn't initialize SDL window: %s\n", SDL_GetError()).c_str());
  }
}

void App::initImGui() {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  io = &ImGui::GetIO();
  (void)io;

  io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  ImGui::StyleColorsDark();

  ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
  ImGui_ImplSDLRenderer3_Init(renderer);
}

};  // namespace Vania

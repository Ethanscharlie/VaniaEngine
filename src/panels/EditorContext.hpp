#pragma once

#include <functional>
#include <vector>

#include "FilesystemWatcher.hpp"
#include "GameDataStructs.hpp"
#include "SDL3/SDL_pixels.h"
#include "SDL3/SDL_render.h"
struct SDL_Renderer;

struct EditorContext {
  GameData gameData;
  FileSystemWatcher filesystemWatcher = {gameData.editorData.rootPath};
  SDL_Window* window;
  SDL_Renderer* renderer;
  std::vector<std::function<void()>> postImGuiRender;
};

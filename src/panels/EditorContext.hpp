#pragma once

#include "FilesystemWatcher.hpp"
#include "GameDataStructs.hpp"
struct SDL_Renderer;

struct EditorContext {
  GameData gameData;
  FileSystemWatcher filesystemWatcher = {gameData.editorData.rootPath};
  SDL_Renderer* renderer;
};

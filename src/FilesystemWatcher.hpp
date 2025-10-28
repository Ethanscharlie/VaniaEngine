#pragma once

#include <filesystem>
#include <vector>

using FileList = std::vector<std::filesystem::path>;

class FileSystemWatcher {
  std::filesystem::path root;
  FileList files;

public:
  FileSystemWatcher(const std::filesystem::path &root);
  void scan();
  FileList getAllFiles();
  FileList getAllFilesWithExtension(const std::string &extension);
};

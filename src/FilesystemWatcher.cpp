#include "FilesystemWatcher.hpp"

FileSystemWatcher::FileSystemWatcher(const std::filesystem::path& root) : root(root) { scan(); }

void FileSystemWatcher::scan() {
  FileList list;

  for (auto const& dir_entry : std::filesystem::recursive_directory_iterator(root)) {
    if (!std::filesystem::is_regular_file(dir_entry)) continue;
    list.push_back(std::filesystem::relative(dir_entry.path(), root));
  }

  files = list;
}

FileList FileSystemWatcher::getAllFiles() { return files; }

FileList FileSystemWatcher::getAllFilesWithExtension(const std::string& extension) {
  FileList list;

  for (const auto& file : files) {
    if (file.extension() != extension) continue;
    list.push_back(file);
  }

  return list;
}

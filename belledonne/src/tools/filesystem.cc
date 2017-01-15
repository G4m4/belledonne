/// @file filesystem.cc
/// @brief Various tools related to filesystem - implementation
/// @author gm
/// @copyright gm 2016
///
/// This file is part of Belledonne
///
/// Belledonne is free software: you can redistribute it and/or modify
/// it under the terms of the GNU General Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.
///
/// Belledonne is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU General Public License for more details.
///
/// You should have received a copy of the GNU General Public License
/// along with Belledonne.  If not, see <http://www.gnu.org/licenses/>.

#include "belledonne/src/configuration.h"

#include <filesystem>

#include "belledonne/src/tools/filesystem.h"

namespace fs = std::tr2::sys;

namespace belledonne {
namespace tools {

void FileSystem::GetAllFiles(const std::string& path,
                             const FileSystem::StringsSet& extensions,
                             std::vector<std::string>& out) {
  const fs::path root_path(path);

#if _MSC_VER < 1900
  for (auto dir_iterator = fs::recursive_directory_iterator(root_path);
       dir_iterator != fs::recursive_directory_iterator();
       ++dir_iterator) {
    const fs::path& directory_entry(*dir_iterator);
    if (fs::exists(directory_entry)
        && fs::is_regular_file(directory_entry)) {
      const fs::path current_path(directory_entry.string());
      const std::string& current_extension(current_path.extension());
      const auto& found = extensions.find(current_extension);
      if (found != extensions.end()) {
        out.push_back(current_path.string());
      }
    }
  }
#else  // _MSC_VER < 1900
  for (auto& directory_entry : fs::recursive_directory_iterator(root_path)) {
    if (fs::exists(directory_entry)
        && fs::is_regular_file(directory_entry)) {
      const fs::path current_path(directory_entry.path());
      const std::string& current_extension(current_path.extension().generic_string());
      const auto& found = extensions.find(current_extension);
      if (found != extensions.end()) {
        out.push_back(current_path.generic_string());
      }
    }
  }
#endif // _MSC_VER < 1900
}

std::string FileSystem::GetParentPath(const std::string& path) {
  const fs::path root_path(path);
#if _MSC_VER < 1900
  return root_path.parent_path().string();
#else  // _MSC_VER < 1900
  return root_path.parent_path().generic_string();
#endif // _MSC_VER < 1900
}

std::string FileSystem::GetCurrentWorkingDir() {
#if _MSC_VER < 1900
  return fs::current_path<fs::path>().string();
#else  // _MSC_VER < 1900
  return fs::current_path().generic_string();
#endif // _MSC_VER < 1900
}

std::string FileSystem::AppendPath(const std::string& base_path, const std::string& appended) {
#if _MSC_VER < 1900
  return (fs::path(base_path) / fs::path(appended)).string();
#else  // _MSC_VER < 1900
  return (fs::path(base_path) / fs::path(appended)).generic_string();
#endif // _MSC_VER < 1900
}

}
}

/// @file filesystem.h
/// @brief Various tools related to filesystem
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

#ifndef BELLEDONNE_SRC_TOOLS_FILESYSTEM_H_
#define BELLEDONNE_SRC_TOOLS_FILESYSTEM_H_

#include <string>
#include <unordered_set>
#include <vector>

namespace belledonne {
namespace tools {

struct FileSystem {
  typedef std::unordered_set<std::string> StringsSet;

  static void GetAllFiles(const std::string& path,
                          const StringsSet& extensions,
                          std::vector<std::string>& out);
  static std::string GetParentPath(const std::string& path);
  static std::string GetCurrentWorkingDir();
  static std::string AppendPath(const std::string& base_path, const std::string& appended);
};

}
}

#endif  // BELLEDONNE_SRC_TOOLS_FILESYSTEM_H_

/// @file tests_tools_filesystem.cc
/// @brief Belledonne Dummy Class
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

#include <string>

#include "gtest/gtest.h"

#include "belledonne/src/tools/filesystem.h"

// Using declarations for tested class
using belledonne::tools::FileSystem;

TEST(ToolsFileSystem, RetrieveFilepaths) {
  const std::string kThisSourcePath(__FILE__);
  const std::string kSourceRootPath(FileSystem::GetParentPath(FileSystem::GetParentPath(FileSystem::GetParentPath(kThisSourcePath))));
  const FileSystem::StringsSet kExtensions({ ".h", ".cc", ".cpp", ".c" });

  std::vector<std::string> all_filepaths;
  FileSystem::GetAllFiles(kSourceRootPath, kExtensions, all_filepaths);
}

/// @file database.h
/// @brief Belledonne main database entry point
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

#ifndef BELLEDONNE_SRC_DATABASE_DATABASE_H_
#define BELLEDONNE_SRC_DATABASE_DATABASE_H_

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>

#include "belledonne/src/common.h"

namespace belledonne {
namespace database {

enum class SymbolType : std::uint8_t {
  kVar = 0,
  kField,
  kFunction,
  kStruct,
  kIgnored,
  kUnknown
};

struct DatabaseEntry {
  DatabaseEntry(const std::string& display_name = "",
                const std::string& file = "",
                SymbolType type = SymbolType::kVar,
                unsigned line = 0,
                unsigned column = 0);
  std::string display_name_;
  std::string file_;
  SymbolType type_;
  unsigned line_;
  unsigned column_;
};

/// @brief "Database" - as in a structure holding all data
class Database {
public:
  typedef std::unordered_map<std::string, DatabaseEntry> Dictionary;

  Database(const std::string& path,
           const std::vector<std::string>& args = { "-x", "c++", "-fno-spell-checking", "-cxx-isystem", "D:/Program Files (x86)/Microsoft Visual Studio 12.0/VC/include" });
  ~Database();

  // Parameters
  void AddIncludePath(const std::string& path);

  // Add data
  void AddPath(const std::string& path);
  void AddPaths(const std::vector<std::string>& paths);

  // Query data
  //std::string GetSymbolStartingWith(const std::string& pattern) const;
  std::string Dump() const;

private:
  void* index_;
  const std::vector<std::string> args_;
  std::vector<std::string> include_paths_;
  std::unique_ptr<class DBManager> data_manager_;
};

}  // namespace database
}  // namespace belledonne

#endif  // BELLEDONNE_SRC_DATABASE_DATABASE_H_

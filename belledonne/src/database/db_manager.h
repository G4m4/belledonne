/// @file db_manager.h
/// @brief Interface with the DBMS
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

#ifndef BELLEDONNE_SRC_DATABASE_DBMANAGER_H_
#define BELLEDONNE_SRC_DATABASE_DBMANAGER_H_

#include <string>

#include "belledonne/src/common.h"

namespace lmdb {
  struct MDB_env;
}

namespace belledonne {
namespace database {

/// @brief The interface with the actual database management system
class DBManager {
public:
  DBManager(const std::string& db_path,
            // LMDB default
            const std::size_t default_map_size = 10485760);
  ~DBManager();

  // Add data
  void Put(const std::string& key, const struct DatabaseEntry& value);

  // Fetch data
  void Get(const std::string& key, DatabaseEntry& value) const;
  std::string Dump() const;

private:
  lmdb::MDB_env* db_env_;
};

}  // namespace database
}  // namespace belledonne

#endif  // BELLEDONNE_SRC_DATABASE_DBMANAGER_H_

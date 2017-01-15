/// @file db_manager.cc
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

#include "belledonne/src/database/db_manager.h"

#include <cstdint>
#include <string>
#include <type_traits>

namespace lmdb {
extern "C" {
#include <lmdb.h>
}
}

#include "belledonne/src/database/database.h"

namespace belledonne {
namespace database {

// @brief Wrapper for base LMDB data type
struct LMDBData {
 public:
   LMDBData(std::size_t size = 0, const void* const data = nullptr)
   {
     // Can't do that in the initializer list - not implemented in MSVC <1900
     val_ = { size, const_cast<void* const>(data) };
   }

   operator lmdb::MDB_val*() {
     return &val_;
   }

   std::string Dump() const {
     return std::string(reinterpret_cast<const char*>(val_.mv_data), static_cast<std::size_t>(val_.mv_size));
   }

 private:
   lmdb::MDB_val val_;
};

DBManager::DBManager(const std::string& db_path, std::size_t default_map_size)
    : db_env_(nullptr) {
  int error_code = lmdb::mdb_env_create(&db_env_);
  BELLEDONNE_ASSERT(error_code == MDB_SUCCESS);
  error_code = lmdb::mdb_env_set_mapsize(db_env_, default_map_size);
  BELLEDONNE_ASSERT(error_code == MDB_SUCCESS);
  // Last argument is opened file mode - ignored on Windows
  error_code = lmdb::mdb_env_open(db_env_, db_path.c_str(), 0, 0664);
  BELLEDONNE_ASSERT(error_code == MDB_SUCCESS);
}

DBManager::~DBManager() {
  mdb_env_close(db_env_);
}

void DBManager::Put(const std::string& key, const DatabaseEntry& value) {
  lmdb::MDB_txn* transaction_handle(nullptr);
  int error_code = lmdb::mdb_txn_begin(db_env_, nullptr, 0, &transaction_handle);
  BELLEDONNE_ASSERT(error_code == MDB_SUCCESS);
  BELLEDONNE_ASSERT(transaction_handle != nullptr);

  lmdb::MDB_dbi dbi;
  error_code = lmdb::mdb_dbi_open(transaction_handle, nullptr, 0, &dbi);
  BELLEDONNE_ASSERT(error_code == MDB_SUCCESS);
  LMDBData key_opaque{ key.size(), key.c_str() };
  LMDBData value_opaque{ sizeof(value), &value };
  // @todo(gm) handle duplicates here?
  error_code = lmdb::mdb_put(transaction_handle, dbi, key_opaque, value_opaque, 0);
  BELLEDONNE_ASSERT(error_code == MDB_SUCCESS);
  error_code = lmdb::mdb_txn_commit(transaction_handle);
  BELLEDONNE_ASSERT(error_code == MDB_SUCCESS);

  lmdb::mdb_dbi_close(db_env_, dbi);
}

void DBManager::Get(const std::string& key, DatabaseEntry& value) const {
  lmdb::MDB_txn* transaction_handle(nullptr);
  int error_code = lmdb::mdb_txn_begin(db_env_,
    nullptr,
    MDB_RDONLY,
    &transaction_handle);
  BELLEDONNE_ASSERT(error_code == MDB_SUCCESS);
  BELLEDONNE_ASSERT(transaction_handle != nullptr);

  lmdb::MDB_dbi dbi;
  error_code = lmdb::mdb_dbi_open(transaction_handle, nullptr, 0, &dbi);
  BELLEDONNE_ASSERT(error_code == MDB_SUCCESS);

  LMDBData key_opaque{ key.size(), key.c_str() };
  LMDBData value_opaque{ sizeof(value), &value };
  error_code = lmdb::mdb_get(transaction_handle, dbi, key_opaque, value_opaque);
  BELLEDONNE_ASSERT(error_code == MDB_SUCCESS);

  lmdb::mdb_dbi_close(db_env_, dbi);
  lmdb::mdb_txn_abort(transaction_handle);
}

std::string DBManager::Dump() const {
  std::string out;

  lmdb::MDB_txn* transaction_handle(nullptr);
  int error_code = lmdb::mdb_txn_begin(db_env_,
                                       nullptr,
                                       MDB_RDONLY,
                                       &transaction_handle);
  BELLEDONNE_ASSERT(error_code == MDB_SUCCESS);

  lmdb::MDB_dbi dbi;
  error_code = lmdb::mdb_dbi_open(transaction_handle, nullptr, 0, &dbi);
  BELLEDONNE_ASSERT(error_code == MDB_SUCCESS);

  lmdb::MDB_cursor* cursor(nullptr);
  error_code = lmdb::mdb_cursor_open(transaction_handle, dbi, &cursor);
  BELLEDONNE_ASSERT(error_code == MDB_SUCCESS);

  LMDBData key_opaque;
  LMDBData value_opaque;
  while (0 == (error_code = lmdb::mdb_cursor_get(cursor,
                                                 key_opaque,
                                                 value_opaque,
                                                 lmdb::MDB_cursor_op::MDB_NEXT))) {
    out += "key: ";
    out += key_opaque.Dump();
    out += "value: ";
    out += value_opaque.Dump();
    out += "\n";
  }
  BELLEDONNE_ASSERT(error_code == MDB_NOTFOUND);
  lmdb::mdb_cursor_close(cursor);
  lmdb::mdb_dbi_close(db_env_, dbi);
  lmdb::mdb_txn_abort(transaction_handle);

  return out;
}

}  // namespace database
}  // namespace belledonne

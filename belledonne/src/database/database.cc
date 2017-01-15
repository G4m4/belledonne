/// @file database.cc
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

#include "belledonne/src/database/database.h"

extern "C" {
#include "clang-c/Index.h"
}

#include "belledonne/src/database/db_manager.h"
#include "belledonne/src/database/visitor.h"

namespace belledonne {
namespace database {

DatabaseEntry::DatabaseEntry(const std::string& display_name,
                             const std::string& file,
                             SymbolType type,
                             unsigned line,
                             unsigned column)
    : display_name_(display_name),
      file_(file),
      type_(type),
      line_(line),
      column_(column) {
  // Nothing to do here for now
}

Database::Database(const std::string& path,
                   const std::vector<std::string>& args)
    : args_(args),
      include_paths_(),
      data_manager_(new DBManager(path)),
      index_(clang_createIndex(0, 0)) {
  BELLEDONNE_ASSERT(index_ != 0);
}

Database::~Database() {
  clang_disposeIndex(index_);
}

void Database::AddIncludePath(const std::string& path) {
  include_paths_.push_back(path);
}

void Database::AddPath(const std::string& path) {
  BELLEDONNE_ASSERT(path.size() > 0);

  const char* args_ptr[64];
  // Create list of arguments - first copy the ones passed from construction
  int args_count = 0;
  for (const std::string& arg : args_) {
    args_ptr[args_count] = arg.c_str();
    args_count += 1;
  }
  // Additional arguments: include paths
  if (include_paths_.size() > 0) {
    args_ptr[args_count] = "-I";
    args_count += 1;
  }
  for (const std::string& current_path : include_paths_) {
    args_ptr[args_count] = current_path.c_str();
    args_count += 1;
  }

  CXTranslationUnit translation_unit = clang_parseTranslationUnit(
    index_,
    path.c_str(),
    &args_ptr[0],
    args_count,
    nullptr,
    0,
    //CXTranslationUnit_DetailedPreprocessingRecord | CXTranslationUnit_SkipFunctionBodies);
    CXTranslationUnit_None);
  BELLEDONNE_ASSERT(translation_unit != nullptr);

  EverythingIndexer visitor(path, *data_manager_.get());
  clang_visitChildren(
    clang_getTranslationUnitCursor(translation_unit),
    &EverythingIndexer::visitorFunction,
    &visitor);
  clang_disposeTranslationUnit(translation_unit);
}

void Database::AddPaths(const std::vector<std::string>& paths) {
  for (const std::string& path : paths) {
    AddPath(path);
  }
}

std::string Database::Dump() const
{
  return data_manager_->Dump();
}

}  // namespace database
}  // namespace belledonne

/// @file visitor.h
/// @brief Visitor passed to Libclang
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

#ifndef BELLEDONNE_SRC_DATABASE_VISITOR_H_
#define BELLEDONNE_SRC_DATABASE_VISITOR_H_

#include <string>

extern "C"
{
#include "clang-c/Index.h"
}

#include "belledonne/src/common.h"
#include "belledonne/src/database/database.h"

namespace belledonne {
namespace database {

class EverythingIndexer {
public:
  EverythingIndexer(const std::string& translation_unit,
                    class DBManager& data_manager);

  CXChildVisitResult visit(CXCursor cursor, CXCursor parent);

  static CXChildVisitResult visitorFunction(
    CXCursor cursor,
    CXCursor parent,
    CXClientData clientData);

 private:
  SymbolType GetSymbolType(CXCursorKind cursor_kind) const;

  const std::string& translation_unit_;
  class DBManager& data_manager_;

  EverythingIndexer& operator=(const EverythingIndexer& lhs) = delete;
};

}  // namespace database
}  // namespace belledonne

#endif  // BELLEDONNE_SRC_DATABASE_VISITOR_H_

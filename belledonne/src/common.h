/// @file common.h
/// @brief Belledonne common utilities header
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

#ifndef BELLEDONNE_SRC_COMMON_H_
#define BELLEDONNE_SRC_COMMON_H_

#include <cassert>

#include "belledonne/src/configuration.h"

namespace belledonne {

/// @brief Ignore unused variables
template<typename Type> void IGNORE(const Type&) {}

/// @brief Assume that the following condition is always true
/// (on some compilers, allows optimization)
#if(_COMPILER_MSVC)
  static inline void ASSUME(const bool condition) {_assume(condition);}
#elif(_COMPILER_GCC)
  static inline void ASSUME(const bool condition) {if (!(condition)) __builtin_unreachable();}
#else
  #define ASSUME(_condition_)
#endif  // _COMPILER_ ?

/// @brief Asserts condition == true
#if(_BUILD_CONFIGURATION_DEBUG)
  #define BELLEDONNE_ASSERT(_condition_) (assert((_condition_)))
#else
  // Maps to "assume" in release configuration for better optimization
  #define BELLEDONNE_ASSERT(_condition_) {::belledonne::ASSUME((_condition_));}
#endif

}  // namespace belledonne

#endif  // BELLEDONNE_SRC_COMMON_H_

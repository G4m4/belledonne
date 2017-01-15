/// @file configuration.h
/// @brief Belledonne configuration file
///
/// Provides basic preprocessor macros in order to detect
/// user variables/hardware/OS/compiler...
///
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

#ifndef BELLEDONNE_SRC_CONFIGURATION_H_
#define BELLEDONNE_SRC_CONFIGURATION_H_

/// @brief Compiler detection
#if(defined(__GNUC__))
  #define _COMPILER_GCC 1
#elif(defined(_MSC_VER))
  #define _COMPILER_MSVC 1
#else
  #error "Compiler could not be detected"
#endif

/// @brief Build configuration detection
/// Since there are no easy cross-platform way to do this,
/// we assume that "no asserts" means release
#if(defined(_NDEBUG) || defined(NDEBUG))
  #define _BUILD_CONFIGURATION_DEBUG 0
#else  // defined(NDEBUG) ?
  #define _BUILD_CONFIGURATION_DEBUG 1
#endif  // defined(NDEBUG) ?

/// @brief Architecture detection - compiler specific preprocessor macros
#if _COMPILER_MSVC
  #if defined(_M_IX86)
    #define _ARCH_X86 1
  #endif
#elif _COMPILER_GCC
  #if (defined(__i386__))
    #define _ARCH_X86 1
  #endif
#endif

#endif  // BELLEDONNE_SRC_CONFIGURATION_H_

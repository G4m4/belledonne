# @brief Build Belledonne tests executable

# preventing warnings from external source files
include_directories(
  SYSTEM
  ${GTEST_INCLUDE_DIRS}
)

include_directories(
  ${BELLEDONNE_INCLUDE_DIR}
)

# Include all subdirectories tests source files
add_subdirectory(database)
add_subdirectory(tools)

source_group("database"
  FILES
  ${BELLEDONNE_TESTS_DATABASE_SRC}
)
source_group("tools"
  FILES
  ${BELLEDONNE_TESTS_TOOLS_SRC}
)

# Source files
set(BELLEDONNE_TESTS_SRC
    main.cc
    ${BELLEDONNE_TESTS_DATABASE_SRC}
    ${BELLEDONNE_TESTS_TOOLS_SRC}
)

# Target
add_executable(belledonne_tests
  ${BELLEDONNE_TESTS_SRC}
)

target_link_libraries(belledonne_tests
  belledonne_lib
  gtest
  ${LIBCLANG_LIBRARIES}
  ${LMDB_LIBRARIES}
  ntdll  # lmdb
  ${MSVC_HACK_LIBS}
)

set_target_mt(belledonne_tests)

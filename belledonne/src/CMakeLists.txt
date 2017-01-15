# @brief Belledonne main source folder cmake file

include_directories(
  ${LIBCLANG_INCLUDE_DIRS}
  ${LMDBCXX_INCLUDE_DIRS}
  ${LMDB_INCLUDE_DIR}
  SYSTEM
)

include_directories(
  ${BELLEDONNE_INCLUDE_DIR}
)

add_subdirectory(database)
add_subdirectory(tools)

source_group("tools"
  FILES
  ${BELLEDONNE_TOOLS_SRC}
  ${BELLEDONNE_TOOLS_HDR}
)
source_group("database"
  FILES
  ${BELLEDONNE_DATABASE_SRC}
  ${BELLEDONNE_DATABASE_HDR}
)

set(BELLEDONNE_SRC
  common.h
  configuration.h
)
set(BELLEDONNE_HDR
)

add_library(belledonne_lib
  ${BELLEDONNE_SRC}
  ${BELLEDONNE_HDR}
  ${BELLEDONNE_DATABASE_SRC}
  ${BELLEDONNE_DATABASE_HDR}
  ${BELLEDONNE_TOOLS_SRC}
  ${BELLEDONNE_TOOLS_HDR}
)

set_target_mt(belledonne_lib)

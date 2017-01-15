# @brief Belledonne standalone root cmake file

set(BELLEDONNE_STANDALONE_DIR
  ${CMAKE_CURRENT_SOURCE_DIR}
)

include_directories(
  ${BELLEDONNE_INCLUDE_DIR}
)

# Look for all standalone-specific files
set (BELLEDONNE_STANDALONE_SRC
  ${BELLEDONNE_STANDALONE_DIR}/main.cc
)
set (BELLEDONNE_STANDALONE_HDR
  # Nothing in here for now
)

source_group("belledonne" FILES ${BELLEDONNE_SRC} ${BELLEDONNE_HDR})
source_group("belledonne_implementation" FILES ${BELLEDONNE_IMPLEMENTATION_SRC} ${BELLEDONNE_IMPLEMENTATION_HDR})
source_group("standalone_specifics" FILES ${BELLEDONNE_STANDALONE_SRC} ${BELLEDONNE_STANDALONE_HDR})

add_executable(belledonne
  ${BELLEDONNE_SRC}
  ${BELLEDONNE_HDR}
  ${BELLEDONNE_IMPLEMENTATION_SRC}
  ${BELLEDONNE_IMPLEMENTATION_HDR}
  ${BELLEDONNE_STANDALONE_SRC}
  ${BELLEDONNE_STANDALONE_HDR}
)

target_link_libraries(belledonne
                      belledonne_lib
)

set_target_mt(belledonne)

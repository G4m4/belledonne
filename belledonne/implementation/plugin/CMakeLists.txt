# @brief Belledonne plugin root cmake file

set(BELLEDONNE_PLUGIN_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}
)

include_directories(
  ${BELLEDONNE_INCLUDE_DIR}
)

source_group("belledonne" FILES ${BELLEDONNE_SRC} ${BELLEDONNE_HDR})
source_group("belledonne_implementation" FILES ${BELLEDONNE_IMPLEMENTATION_SRC} ${BELLEDONNE_IMPLEMENTATION_HDR})
source_group("plugin_specifics" FILES ${BELLEDONNE_PLUGIN_SRC} ${BELLEDONNE_PLUGIN_HDR})

add_library(belledonne_plugin SHARED
  ${BELLEDONNE_SRC}
  ${BELLEDONNE_HDR}
  ${BELLEDONNE_IMPLEMENTATION_SRC}
  ${BELLEDONNE_IMPLEMENTATION_HDR}
  ${BELLEDONNE_PLUGIN_SRC}
  ${BELLEDONNE_PLUGIN_HDR}
)

target_link_libraries(belledonne_plugin
                      belledonne_lib)
